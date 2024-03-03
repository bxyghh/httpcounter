#include <unistd.h>  // for close
#include <stdarg.h>  // for va_start, va_end
#include <thread>    // for std::thread
#include <string>    // for std::string
#include <map>       // for std::map
#include <istream>   // for std::istringstream
#include <sstream>   // for std::stringstream

#include "HttpServer.h"

static int writefull(int fd, const char *data, size_t len)
{
    int n = 0;
    int c = 0;
    do {
        n += c;
        c = write(fd, data+n, len-n);
    } while (n < (int)len);
    return n;
}

void HttpResponseWriter::SetStatus(int statusCode, const std::string &statusText)
{
    _statusCode = statusCode;
    _statusText = statusText;
}

void HttpResponseWriter::AddHeader(const std::string &key, const std::string &value)
{
    _headers[key] = value;
}

void HttpResponseWriter::SetBody(const std::string &body)
{
    _body = body;
}

void HttpResponseWriter::SetBodyF(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    _body = buffer;
}

void HttpResponseWriter::Flush()
{
    std::string buffer;
    if (_headers.find("Content-Length") == _headers.end()) {
        _headers["Content-Length"] = std::to_string(_body.size());
    }
    if (_headers.find("Content-Type") == _headers.end()) {
        _headers["Content-Type"] = "text/plain";
    }
    buffer.append("HTTP/1.1 " + std::to_string(_statusCode) + " " + _statusText + "\r\n");
    for (auto &kv : _headers) {
        buffer.append(kv.first + ": " + kv.second + "\r\n");
    }
    buffer.append("\r\n");
    buffer.append(_body);
    writefull(_fd, buffer.c_str(), buffer.size());
}

void HttpServer::ServeNewConnection(int fd)
{
    std::thread([this, fd] {
        this->serveNewConnection(fd);
    }).detach();
}

void HttpServer::serveNewConnection(int fd)
{
    char buffer[1024];
    int n = read(fd, buffer, sizeof(buffer));
    if (n <= 0) {
        close(fd);
        return;
    }
    buffer[n] = 0;
    std::string data(buffer, n);
    size_t pos = data.find("\r\n\r\n");
    if (pos == std::string::npos) {
        close(fd);
        return;
    }
    std::string header = data.substr(0, pos);
    std::string body = data.substr(pos + 4);

    std::string method, uri, version;
    std::map<std::string, std::string> headers;
    std::istringstream iss(header);
    iss >> method >> uri >> version;
    std::string line;
    while (std::getline(iss, line)) {
        if (line.empty()) {
            break;
        }
        size_t pos = line.find(":");
        if (pos == std::string::npos) {
            continue;
        }
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        headers[key] = value;
    }
    HttpRequest request(method, uri, version, headers, body);
    HttpResponseWriter writer(fd);
    _handler->ServeRequest(request, writer);
    close(fd);
}