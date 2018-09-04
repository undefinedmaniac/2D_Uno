#include "liststreamsocket.h"

namespace network
{

ListStreamSocket::ListStreamSocket(tcp::socket *socket) : socket_(socket)
{

}

tcp::socket *ListStreamSocket::getSocket()
{
    return socket_;
}

const tcp::socket *ListStreamSocket::getSocket() const
{
    return socket_;
}

void ListStreamSocket::asyncRead(ListStreamSocket::readCallback callback)
{
    callback_ = callback;

    socket_->async_read_some(
                boost::asio::buffer(readBuffer_),
                boost::bind(&ListStreamSocket::handleRead, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
}

void ListStreamSocket::handleRead(const boost::system::error_code &error,
                                  size_t bytes_transferred)
{
    const unsigned long long oldLength = messageBuffer_.size();

    const unsigned long long newSize = oldLength + bytes_transferred;

    if (newSize > 5120) {
        abortWithResult(Result::ContentTooLarge);
        return;
    }

    messageBuffer_.reserve(newSize);

    for (const char &character : readBuffer_)
        messageBuffer_.push_back(character);

    if (messageHeader_.empty()) {
        const unsigned long long size = messageBuffer_.size();
        for (; spaceSearchPos_ < size; spaceSearchPos_++) {
            if (messageBuffer_[spaceSearchPos_] == ' ')
                break;
        }

        if (spaceSearchPos_ == size)
            return;

        if (!fetchHeader()) {
            abortWithResult(Result::InvalidHeader);
            return;
        }

        messageLength_ = spaceSearchPos_ + messageHeader_.back() + 1;

        if (messageLength_ > 5120) {
            abortWithResult(Result::ContentTooLarge);
            return;
        }
    }

    if (messageBuffer_.size() < messageLength_)
        return;

    string messageContent = messageBuffer_.substr(spaceSearchPos_ + 1,
                                                  messageHeader_.back());

    const unsigned long long headerSize = messageHeader_.size();

    vector<string> list(headerSize);

    for (unsigned int i = 0; i < headerSize; i++) {
        unsigned int startPos, endPos, length;

        if (i == 0)
            startPos = 0;
        else
            startPos = messageHeader_[i - 1];

        if (i == headerSize - 1)
            endPos = static_cast<unsigned int>(messageContent.size());
        else
            endPos = messageHeader_[i];

        length = endPos - startPos;

        if (startPos < headerSize &&
            endPos < headerSize && length <= headerSize) {
            list.push_back(messageContent.substr(startPos, length));
        }
        else {
            abortWithResult(Result::InvalidHeader);
            return;
        }
    }

    resetData(messageLength_);

    if (callback_)
        callback_(Result::Success, list);
}

bool ListStreamSocket::fetchHeader()
{
    string header = messageBuffer_.substr(0, spaceSearchPos_);
    vector<string> headerData;

    //Split the string by '.'
    istringstream stream(header);
    string data;
    while (std::getline(stream, data, '.'))
        headerData.push_back(data);

    if (!headerData.empty()) {
        messageHeader_.resize(headerData.size());

        for (unsigned int i = 0; i < headerData.size(); i++) {
            try {
                int number = std::stoi(headerData[i]);

                if (number > 0)
                    messageHeader_[i] = static_cast<unsigned int>(number);
                else
                    return false;
            } catch (std::exception error) {
                return false;
            }
        }

        return true;
    }

    return false;
}

void ListStreamSocket::resetData(unsigned int length)
{
    messageBuffer_.erase(0, length);
    messageHeader_.clear();
    messageHeader_.resize(0);
    spaceSearchPos_ = 0;
}

void ListStreamSocket::abortWithResult(ListStreamSocket::Result result)
{
    if (callback_)
        callback_(result, vector<string>());
    resetData(static_cast<unsigned int>(messageBuffer_.size()));
}

}
