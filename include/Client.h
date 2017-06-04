//
// Created by saltuk on 12/22/16.
//

#ifndef THREADPROXY_CLIENT_H
#define THREADPROXY_CLIENT_H


#include <cstddef>
#include "CacheManager.h"

class Client {
public:
    Client(int fd, CacheManager *cache);
    ~Client();
    void work();

private:
    static constexpr size_t buff_size = 1024*500;
    char *buff;

    int socket;
    CacheManager *cache;

    static ThreadSafeDigit<int> clientsCount;

};


#endif //THREADPROXY_CLIENT_H
