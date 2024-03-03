#include "HttpCounter.h"

void HttpCounter::ServeRequest(const HttpRequest &r, HttpResponseWriter &w)
{
    if (r.Uri() == "/incr" && r.Method() == "POST") {
        _counter->Increment();
        w.SetStatus(200, "OK");
        w.SetBody("Counter incremented\r\n");
    }
    else if (r.Uri() == "/value" && r.Method() == "GET") {
        int value = _counter->Value();
        w.SetStatus(200, "OK");
        w.SetBodyF("Counter value: %d\r\n", value);
    }
    else {
        w.SetStatus(404, "Not Found");
        w.SetBody("Not Found\r\n");
    }
    w.Flush();
}