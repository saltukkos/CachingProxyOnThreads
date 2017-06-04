//
// Created by saltuk on 12/15/16.
//

#include "CacheEntry.h"
#include "CacheManager.h"
#include "Downloader.h"
#include <algorithm>

long CacheEntry::getLastUse() {
    return lastUse;
}

void CacheEntry::setLastUse(long lastUse) {
    CacheEntry::lastUse = lastUse;
}

CacheEntry::CacheEntry(ThreadSafeDigit<long> *digit) :
    cacheSize(digit),
    state(Empty),
    lastUse(0),
    clients(0),
    mutex(),
    condVar(mutex){}

unsigned long CacheEntry::clientsCount() {
    MutexGuard guard(mutex);
    return (unsigned long) clients;
}

void CacheEntry::deleteClient() {
    MutexGuard guard(mutex);
    --clients;
    assert(clients >= 0);
}

void CacheEntry::addClient() {
    MutexGuard guard(mutex);
    ++clients;
   // std::cerr << "push new client, size: " << clients << std::endl;
}

CacheEntry::~CacheEntry() {
    MutexGuard guard(mutex);
  //  std::cerr << "delete entry " << this << " decrease cache on " << data.size() << std::endl;
    cacheSize->atomicChange(-data.size());
  //  std::cerr << "decrease " << data.size() << " bytes from cache, size is " << cacheSize->get() << std::endl;

    assert(cacheSize->get() >= 0);
}

ssize_t CacheEntry::getData(size_t offset, char *buf, size_t len) {
    MutexGuard guard(mutex);

    assert(offset <= data.size());
    while (offset == data.size()){
        if (state == Finished || state == Broken)
            return -1;
        condVar.wait();
    }

    size_t available = data.size() - offset;
    if (len > available)
        len = available;

    for (size_t i = 0; i < len; ++i){
        buf[i] = data[i + offset];
    }

    return len;
}

void CacheEntry::writeData(const char *buf, size_t len) {
    MutexGuard guard(mutex);

    cacheSize->atomicChange(len);
 //   std::cerr << this << " push " << len << " bytes to cache, size is " << cacheSize->get() <<  " (my size is " << data.size() << ")" <<std::endl;
    for (int i = 0; i < len; ++i)
        data.push_back(buf[i]);
    condVar.notifyAll();
}

struct Arg{
    Arg(shared_ptr<CacheEntry> entry, CacheManager *manager, Request *request) :
            entry(entry),
            manager(manager),
            request(request) {}

    shared_ptr<CacheEntry> entry;
    CacheManager *manager;
    Request *request;
};

void *runDownloader(void *data){
    Arg *arg = (Arg *)data;
    Downloader *downloader = nullptr;

    try {
        downloader = new Downloader(arg->entry, arg->manager, arg->request);
        downloader->run();
    } catch (std::exception &e){
        std::cerr << e.what() << std::endl;
    }

    delete arg;
    delete downloader;
    return nullptr;
}

void CacheEntry::createDownloaderIfNotExists(shared_ptr<CacheEntry> ptr, Request *request, CacheManager *cache) {
    MutexGuard guard(mutex);

    if (state != Empty)
        return;

    pthread_t thread;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    Arg *arg = new Arg(ptr, cache, request);

    pthread_create(&thread, &attr, runDownloader, arg);
    pthread_attr_destroy(&attr);

    state = Loading;
}

CacheEntry::State CacheEntry::getState() {
    MutexGuard guard(mutex);
    return state;
}

void CacheEntry::setState(CacheEntry::State state) {
    MutexGuard guard(mutex);
    this->state = state;
    condVar.notifyAll();
}

unsigned long CacheEntry::getSize() {
    MutexGuard guard(mutex);
    return data.size();
}

const char *CacheEntry::getP() {
    return &data[0];
}
