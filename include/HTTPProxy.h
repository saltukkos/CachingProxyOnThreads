//
// Created by saltuk on 12/22/16.
//

#ifndef THREADPROXY_HTTPPROXY_H
#define THREADPROXY_HTTPPROXY_H

#include "ThreadProxy.h"
#include "CacheManager.h"

class HTTPProxy {
public:
    HTTPProxy(int port, bool &);
    void run();

    ~HTTPProxy();

private:
    int fd;
    bool &working;
    pthread_attr_t attr;
    CacheManager cache;
};


#endif //THREADPROXY_HTTPPROXY_H
