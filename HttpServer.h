#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <string>
#include <map>

#include "TcpServer.h"

class HttpRequest {
public:
    HttpRequest(std::string method, std::string uri, std::string version, std::map<std::string, std::string> headers, std::string body)
        : _method(method), _uri(uri), _version(version), _headers(headers), _body(body) {}
    std::string Method() const { return _method; }
    std::string Uri() const { return _uri; }
    std::string Version() const { return _version; }
    std::string Body() const { return _body; }
    std::string Header(const std::string &key) const {
        auto it = _headers.find(key);
        if (it != _headers.end()) {
            return it->second;
        }
        return "";
    }
private:
    std::string _method;
    std::string _uri;
    std::string _version;
    std::map<std::string, std::string> _headers;
    std::string _body;
};

class HttpResponseWriter {
public:
    HttpResponseWriter(int fd):_fd(fd) {}
    HttpResponseWriter() = delete;
    HttpResponseWriter(const HttpResponseWriter &) = delete;
    ~HttpResponseWriter() = default;

    void SetStatus(int statusCode, const std::string &statusText);
    void AddHeader(const std::string &key, const std::string &value);
    void SetBody(const std::string &body);
    void SetBodyF(const char *format, ...);
    void Flush();
private:
    int _fd;
    int _statusCode;
    std::string _statusText;
    std::map<std::string, std::string> _headers;
    std::string _body;
};


class HttpHandler {
public:
    virtual ~HttpHandler() = default;
    virtual void ServeRequest(const HttpRequest &r, HttpResponseWriter &w) = 0;
};


class HttpServer :public TcpHandler {
public:
    HttpServer(HttpHandler *handler):_handler(handler) {}
    void ServeNewConnection(int fd) override;
private:
    void serveNewConnection(int fd);
    HttpHandler *_handler;
};

#endif // #ifndef HTTP_SERVER_H