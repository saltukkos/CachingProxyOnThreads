//
// Created by saltuk on 12/22/16.
//

#include <netinet/in.h>
#include <unistd.h>
#include "HTTPProxy.h"
#include "Client.h"

HTTPProxy::HTTPProxy(int port, bool &working) :
    working(working){
    sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons((uint16_t)port);

    fd = socket(AF_INET, SOCK_STREAM, 0);

    if (bind(fd, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        perror("bind");
        throw std::invalid_argument("unable to bind on this address");
    }

    listen(fd, 50);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
}

HTTPProxy::~HTTPProxy() {
    close(fd);
    pthread_attr_destroy(&attr);
    std::cerr << "Proxy stopped" << std::endl;
}

struct Arg{
    Arg(int fd, CacheManager *manager):
        fd(fd),
        manager(manager) {}
    int fd;
    CacheManager *manager;
};

void *clientHandler(void *data){
    Arg *arg = (Arg *)data;

    Client *client = nullptr;
    try{
        client = new Client(arg->fd, arg->manager);
        client->work();
    } catch (std::exception &e){
        std::cerr << e.what() << std::endl;
    }

    delete client;
    delete arg;
    return nullptr;
}

void HTTPProxy::run() {
    pthread_t thread;
    std::cerr << "Proxy started" << std::endl;
    while (working) {
        int client = accept(fd, NULL, NULL);
        if (client < 0)
            continue;
        std::cerr << "new client accepted" << std::endl;
        Arg *arg = new Arg(client, &cache);
        pthread_create(&thread, &attr, clientHandler, arg);
    }
}
