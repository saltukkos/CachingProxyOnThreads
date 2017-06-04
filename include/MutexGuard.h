//
// Created by saltuk on 12/15/16.
//

#ifndef THREADPROXY_MUTEXGUARD_H
#define THREADPROXY_MUTEXGUARD_H

#include "ThreadProxy.h"
#include "Mutex.h"

class MutexGuard {
public:
    MutexGuard(Mutex&);
    ~MutexGuard();
private:
    Mutex &mutex;
};


#endif //THREADPROXY_MUTEXGUARD_H
