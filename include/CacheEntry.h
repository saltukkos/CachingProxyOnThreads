//
// Created by saltuk on 12/15/16.
//

#ifndef THREADPROXY_CACHEENTRY_H
#define THREADPROXY_CACHEENTRY_H

#include <vector>
#include "ThreadProxy.h"
#include "ThreadSafeDigit.h"
#include "CondVar.h"
#include "Request.h"
#include "CacheManager.h"

class Client;

class CacheEntry {
public:

    enum State{
        Empty, Loading, Finished, Broken
    };

    CacheEntry(ThreadSafeDigit<long> *digit);
    CacheEntry(CacheEntry &other) = delete;
    CacheEntry& operator=(CacheEntry&) = delete;
    ~CacheEntry();

    void addClient();
    void deleteClient();
    void createDownloaderIfNotExists(shared_ptr<CacheEntry> ptr, Request *request, class CacheManager *cache);
    unsigned long clientsCount();

    ssize_t getData(size_t offset, char *buf, size_t len);
    void writeData(const char *buf, size_t len);

    long getLastUse();
    void setLastUse(long lastUse);

    State getState();
    void setState(State state);

    unsigned long getSize();
    const char* getP();

private:
    long clients;
    long lastUse;
    ThreadSafeDigit<long> *cacheSize;
    State state;
    std::vector<char> data;

    /* !!! ORDER IMPORTANT !!! */
    Mutex mutex;
    CondVar condVar;
    /* !!! END ORDER IMPORTANT !!! */
};


#endif //THREADPROXY_CACHEENTRY_H
