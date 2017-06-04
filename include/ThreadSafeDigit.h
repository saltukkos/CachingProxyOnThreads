//
// Created by saltuk on 12/19/16.
//

#ifndef THREADPROXY_THREADSAFEDIGIT_H
#define THREADPROXY_THREADSAFEDIGIT_H


#include "Mutex.h"
#include "MutexGuard.h"

template <typename T>
class ThreadSafeDigit {
public:
    ThreadSafeDigit() : val(){}
    ThreadSafeDigit(const T& val) : val(val){}

    ThreadSafeDigit(ThreadSafeDigit&) = delete;
    ThreadSafeDigit* operator=(ThreadSafeDigit&) = delete;

    ~ThreadSafeDigit() = default;

    const T& get() {
        MutexGuard guard(mutex);
        return val;
    }

    void set(const T& val) {
        MutexGuard guard(mutex);
        this->val = val;
    }

    void atomicChange(const T& diff) {
        MutexGuard guard(mutex);
        val += diff;
  //      std::cerr << "change size on " << diff << " now: " << val <<std::endl;
    }

private:
    Mutex mutex;
    T val;
};


#endif //THREADPROXY_THREADSAFEDIGIT_H
