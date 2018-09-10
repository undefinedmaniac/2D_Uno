#ifndef LISTSTREAMSOCKET_H
#define LISTSTREAMSOCKET_H

#include <memory>
#include <deque>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

using boost::asio::ip::tcp;

typedef std::shared_ptr<tcp::socket> shared_socket;

/**
 * @brief The ListStreamSocket class
 *
 * Wraps a boost tcp socket to provide an interface for receiving stringlists
 * instead of raw bytes.
 */
class ListStreamSocket
{
public:
    enum Result {
        Success = 0, // The operation completed successfully
        BufferOverflow, // The message exceeded the maximum size
        InvalidHeader, // The message header was not valid
        EndOfFile, // The EOF was reached for the socket
        UnknownError // An unknown error occurred
    };

    typedef std::function<void(ListStreamSocket*,
                               Result,
                               std::vector<std::string>)> callback;

    /**
     * @brief ListStreamSocket Create a new ListStreamSocket
     * @param socket The socket to wrap
     */
    ListStreamSocket(shared_socket socket);

    /**
     * @brief getSocket Get the socket
     * @return The socket
     */
    tcp::socket *getSocket();

    /**
     * @brief getMessage Fetch a single message
     * @param callback The callback function
     *
     * Returns a single message to the callback function. If no messages are
     * readily available, the function will asyncronously wait for messages.
     */
    void getMessage(callback callback);

    /**
     * @brief setBufferLength Set the read buffer length
     * @param length The length
     *
     * This is the maximum amount that is read from the socket at a time.
     * This value should be less than the MaxMessageLength.
     *
     * Default is 1024 bytes
     */
    void setBufferLength(unsigned int length);

    /**
     * @brief setMaxMessageLength Set the maximum message length
     * @param length The length
     *
     * This is the maximum size of a single message.
     * This value should be greater than the read buffer length
     *
     * Default is 5120 bytes
     */
    void setMaxMessageLength(unsigned int length);

    /**
     * @brief getBufferLength Get the read buffer length
     * @return The length
     */
    unsigned int getBufferLength() const;

    /**
     * @brief getMaxMessageLength Get the maximum message length
     * @return The length
     */
    unsigned int getMaxMessageLength() const;

private:
    shared_socket socket_;
    callback callback_;

    std::deque<std::vector<std::string>> messageQueue_;

    unsigned int bufferLength_ = 1024,
                 maxMessageLength_ = 5120;

    std::string messageBuffer_;
    std::vector<unsigned int> headerData_;
    unsigned long long messageLength_,
                       contentStartPos_;

    void readHandler(const boost::system::error_code& error,
                     std::size_t charsRead,
                     std::shared_ptr<char> data);

    bool processNewData(unsigned long long oldSize,
                        unsigned long long newSize);

    bool fetchHeader(size_t length);

    std::vector<std::string> grabContent(const std::string &content);

    void removeMessage();
    void resetData();

    void abort(Result result);
};

#endif // LISTSTREAMSOCKET_H
