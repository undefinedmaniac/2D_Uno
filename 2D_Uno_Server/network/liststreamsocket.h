#ifndef LISTSTREAMSOCKET_H
#define LISTSTREAMSOCKET_H

#include <memory>
#include <vector>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

using std::shared_ptr;
using std::vector;
using std::string;
using std::istringstream;

using boost::asio::ip::tcp;

namespace network
{

class ListStreamSocket
{
public:
    enum class Result {
        Success,         // The operation completed successfully
        InvalidHeader,   // The header in the message was invalid
        ContentTooLarge  // The content of the message was too large
    };

    typedef std::function<void(ListStreamSocket::Result, std::vector<std::string>)> readCallback;

    ListStreamSocket(tcp::socket *socket);

    tcp::socket *getSocket();
    const tcp::socket *getSocket() const;

    void asyncRead(readCallback callback);
private:
    void handleRead(const boost::system::error_code& error,
                      size_t bytes_transferred);
    bool fetchHeader();
    void resetData(unsigned int length);
    void abortWithResult(Result result);

    char readBuffer_[1024];
    string messageBuffer_;
    vector<unsigned int> messageHeader_;
    unsigned int messageLength_, spacePos_, spaceSearchPos_ = 0;

    tcp::socket *socket_;
    readCallback callback_ = nullptr;
};

}

#endif // LISTSTREAMSOCKET_H
