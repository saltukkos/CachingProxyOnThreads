//
// Created by saltuk on 12/22/16.
//

#ifndef THREADPROXY_DOWNLOADER_H
#define THREADPROXY_DOWNLOADER_H


#include "ThreadProxy.h"
#include "CacheEntry.h"
#include "CacheManager.h"

class Downloader {
public:
    Downloader(shared_ptr<CacheEntry> entry, CacheManager *cache, Request *request);
    ~Downloader();
    void run();

private:
    static constexpr int buffSize = 1024*500;
    char *buff;

    CacheManager *cache;
    Request request;
    int fd;
    shared_ptr<CacheEntry> entry;
};


#endif //THREADPROXY_DOWNLOADER_H
