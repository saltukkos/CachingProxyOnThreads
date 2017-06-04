//
// Created by saltuk on 12/22/16.
//

#include <unistd.h>
#include <iostream>
#include "Client.h"
#include "Request.h"
#include "Response.h"

Client::Client(int fd, CacheManager *cache) :
    buff(new char[buff_size]),
    socket(fd),
    cache(cache){
    clientsCount.atomicChange(1);
    std::cerr << "new client " << this <<", count : " << clientsCount.get() << std::endl;
}

Client::~Client() {
    close(socket);
    clientsCount.atomicChange(-1);
    std::cerr << "client " << this << " deleted, count: " << clientsCount.get() << std::endl;
    delete [] buff;
}

void Client::work() {
    bool ready;
    Request request;
    std::cerr << "client " << this << " starts" << std::endl;

    do {
        ssize_t size = read(socket, buff, buff_size);
        if (size <= 0) {
            return;
        }
        ready = request.putData(buff, 0, (size_t) size);
    } while (!ready);

    std::cerr << "client " << this << " finished reading from browser" << std::endl;

    shared_ptr<CacheEntry> entry = cache->getEntry(request.getHost() + request.getRequest());
    entry->createDownloaderIfNotExists(entry, &request, cache);
    entry->addClient();


    size_t offset = 0;
    try {
        for (;;) {
            auto ret = entry->getData(offset, buff, buff_size);
            if (ret < 0) {
                break;
            }
            offset += ret;
            write(socket, buff, (size_t) ret);
        }
    } catch (std::exception& e){
        std::cerr << "client " << this << " finished" << std::endl;
        entry->deleteClient();
        throw e;
    }
    std::cerr << "client " << this << " finished" << std::endl;
    entry->deleteClient();

}

ThreadSafeDigit<int> Client::clientsCount(0);