#include "liststreamsocket.h"

ListStreamSocket::ListStreamSocket(shared_socket socket) :
    socket_(socket)
{
}

tcp::socket *ListStreamSocket::getSocket()
{
    return socket_.get();
}

void ListStreamSocket::getMessage(ListStreamSocket::callback callback)
{
    if (messageQueue_.empty()) {
        callback_ = callback;

        std::shared_ptr<char> buffer(new char[bufferLength_],
                                     std::default_delete<char[]>());

        socket_->async_read_some(boost::asio::buffer(buffer.get(), bufferLength_),
                                 boost::bind(&ListStreamSocket::readHandler,
                                             this,
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred,
                                             buffer));
    }
    else {
        std::vector<std::string> message = messageQueue_.front();
        messageQueue_.pop_front();

        callback(this, Result::Success, message);
    }
}

void ListStreamSocket::setBufferLength(unsigned int length)
{
    if (length <= maxMessageLength_)
        bufferLength_ = length;
}

void ListStreamSocket::setMaxMessageLength(unsigned int length)
{
    if (length >= bufferLength_)
        maxMessageLength_ = length;
}

unsigned int ListStreamSocket::getBufferLength() const
{
    return bufferLength_;
}

unsigned int ListStreamSocket::getMaxMessageLength() const
{
    return maxMessageLength_;
}

void ListStreamSocket::readHandler(const boost::system::error_code &error,
                                   std::size_t charsRead,
                                   std::shared_ptr<char> data)
{
    if (!error) {
        // Dump the new characters into the main buffer
        const unsigned long long oldSize = messageBuffer_.size();
        const unsigned long long newSize = oldSize + charsRead;

        if (newSize <= maxMessageLength_) {
            messageBuffer_.reserve(messageBuffer_.size() + charsRead);
            messageBuffer_.append(data.get(), charsRead);

            if (!processNewData(oldSize, newSize))
                getMessage(callback_);
        }
        else {
            // The new size was too big!
            abort(Result::BufferOverflow);
        }
    }
    else {
        // We had some problems reading from the socket
        if (error == boost::asio::error::eof)
            abort(Result::EndOfFile);
        else
            abort(Result::UnknownError);
    }
}

bool ListStreamSocket::processNewData(unsigned long long oldSize,
                                      unsigned long long newSize)
{   
    bool removedMessage = false;

    while (true) {
        if (headerData_.empty()) {
            // Search for the end of the header
            const size_t position = messageBuffer_.find(' ', oldSize);

            // We found it - position is the length of the header data
            if (position != std::string::npos) {
                // Fetch the header
                if (!fetchHeader(position)) {
                    abort(Result::InvalidHeader);
                    return removedMessage;
                }
            }
            else {
                // Leave if we don't have the whole header yet
                return removedMessage;
            }

            // Calculate content start pos and total message length
            contentStartPos_ = position + 1;
            messageLength_ = contentStartPos_ + headerData_.back();
        }

        // Leave if we don't have the entire message yet
        if (newSize < messageLength_)
            return removedMessage;

        // Grab the content of the message
        std::vector<std::string> contentList =
                grabContent(messageBuffer_.substr(contentStartPos_));

        removedMessage = true;

        callback_(this, Result::Success, contentList);

        removeMessage();
    }
}

bool ListStreamSocket::fetchHeader(size_t length)
{
    const std::string headerString = messageBuffer_.substr(0, length);

    // Split the header apart
    std::vector<std::string> splitData;
    boost::algorithm::split(splitData,
                            headerString,
                            boost::algorithm::is_any_of("."));

    // Prepare to fill the headerData vector
    const size_t splitSize = splitData.size();

    // Check if we have any header data
    if (splitSize == 0)
        return false;

    headerData_.resize(splitSize);

    bool dataIsValid = true;
    for (unsigned int i = 0; i < splitSize; i++) {
        try {
            unsigned long number = std::stoul(splitData[i]);  

            // If the number is greater than 0 and greater than the last number
            // read
            if (number > 0 && (i == 0 || number > headerData_[i - 1])) {
                headerData_[i] = number;
            }
            else {
                dataIsValid = false;
                break;
            }
        }
        catch (...) {
            dataIsValid = false;
            break;
        }
    }

    return dataIsValid;
}

std::vector<std::string> ListStreamSocket::grabContent(const std::string &content)
{
    size_t contentSize = headerData_.size();

    std::vector<std::string> splitContent;
    splitContent.reserve(contentSize);

    unsigned int startPos, endPos, length;

    // Go through the content picking each string out
    // based on its starting and ending position
    for (unsigned int i = 0; i < contentSize; i++) {
        startPos = (i == 0 ? 0 : headerData_[i - 1]);
        endPos = headerData_[i];
        length = endPos - startPos;

        splitContent.push_back(content.substr(startPos, length));
    }

    return splitContent;
}

void ListStreamSocket::removeMessage()
{
    messageBuffer_.erase(0, messageLength_);
    headerData_.clear();
}

void ListStreamSocket::resetData()
{
    messageBuffer_.clear();
    headerData_.clear();
}

void ListStreamSocket::abort(ListStreamSocket::Result result)
{
    resetData();
    callback_(this, result, std::vector<std::string>());
}
