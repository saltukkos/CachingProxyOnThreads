//
// Created by saltuk on 12/15/16.
//

#ifndef THREADPROXY_CACHEMANAGER_H
#define THREADPROXY_CACHEMANAGER_H


#include <map>
#include "Mutex.h"
#include "CacheEntry.h"
#include "ThreadSafeDigit.h"

class CacheManager {
public:
    CacheManager();

    typedef std::string key_t;
    typedef shared_ptr<class CacheEntry> val_t;
    
    val_t getEntry(const key_t &key);
    void detachEntry(const key_t &key);
private:
    std::map<key_t, val_t> map;
    Mutex mutex;

    long lastUse;
    ThreadSafeDigit<long> currentSize;

    static constexpr long maxSize = 1024*1024*1536;
    static double garbageLimit;

    CacheEntry* createNode();
};


#endif //THREADPROXY_CACHEMANAGER_H
