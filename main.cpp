#include <unistd.h>  // for getopt

#include "TcpServer.h"
#include "HttpServer.h"
#include "HttpCounter.h"
#include "MemoryCounter.h"


int main(int argc, char *argv[])
{
    const char *host = nullptr;
    short port = 0;
    
    int optind = 0;
    while((optind = getopt(argc, argv, "h:p:")) != -1) {
        switch (optind) {
            case 'h': host = optarg; break;
            case 'p': port = atoi(optarg); break;
            default:
                fprintf(stderr, "Usage: %s [-h host] [-p port]\n", argv[0]);
                return 1;
        }
    }

    if (host == nullptr) {
        host = "127.0.0.1";
    }
    if (port == 0) {
        port = 8080;
    }

    MemoryCounter memoryCounter;
    HttpCounter httpCounter(&memoryCounter);
    HttpServer httpServer(&httpCounter);
    TcpServer tcpServer(&httpServer);

    int backlog = 10;
    tcpServer.ListenAndServe(host, port, backlog);
    return 0;
}