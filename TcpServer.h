#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_
class TcpHandler {
public:
    virtual ~TcpHandler() = default;
    // ServeNewConnection process new tcp socket connection
    // fd: file descriptor of the new connection
    // ServeNewConnection should close the fd when it's done
    virtual void ServeNewConnection(int fd) = 0;
};

class TcpServer {
public:
    TcpServer() = delete;
    TcpServer(TcpHandler *handler):_handler(handler){};
    TcpServer(TcpServer &&) = delete;
    ~TcpServer() = default;

    int ListenAndServe(const char *host, short port, int backlog);
private:
    bool _stop = false;
    TcpHandler *_handler;
};
#endif // #ifndef TCP_SERVER_H_