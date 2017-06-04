//
// Created by saltuk on 12/15/16.
//

#ifndef THREADPROXY_MUTEX_H
#define THREADPROXY_MUTEX_H


#include "ThreadProxy.h"

class Mutex {
public:
    Mutex();
    Mutex(Mutex &other) = delete;
    Mutex &operator=(Mutex &) = delete;

    ~Mutex();
    void lock();
    void unlock();

    friend class CondVar;

private:
    pthread_mutex_t mutex;
    static pthread_mutexattr_t *attr;
};


#endif //THREADPROXY_MUTEX_H
