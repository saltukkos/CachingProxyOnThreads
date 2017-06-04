//
// Created by saltuk on 12/15/16.
//

#include "MutexGuard.h"

MutexGuard::MutexGuard(Mutex &mutex) :
    mutex(mutex) {
    mutex.lock();
}

MutexGuard::~MutexGuard() {
    mutex.unlock();
}
