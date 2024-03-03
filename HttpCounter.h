#ifndef HTTP_COUNTER_H
#define HTTP_COUNTER_H

#include "HttpServer.h"

class Counter {
public:
    virtual ~Counter() = default;
    virtual void Increment() = 0;
    virtual int Value() const  = 0;
};

class HttpCounter : public HttpHandler {
public:
    HttpCounter(Counter *counter):_counter(counter) {}
    HttpCounter() = delete;
    ~HttpCounter() = default;

    void ServeRequest(const HttpRequest &r, HttpResponseWriter &w) override;
private:
    Counter *_counter;
};
#endif // #ifndef HTTP_COUNTER_H