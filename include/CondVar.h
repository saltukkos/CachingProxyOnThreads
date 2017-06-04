//
// Created by saltuk on 12/22/16.
//

#ifndef THREADPROXY_CONDVAR_H
#define THREADPROXY_CONDVAR_H

#include "ThreadProxy.h"
#include "Mutex.h"

class CondVar {
public:
    CondVar(Mutex &mutex);
    CondVar(CondVar &other) = delete;
    CondVar&operator=(CondVar&) = delete;

    ~CondVar();

    void wait();

    void notify();

    void notifyAll();

private:

    class Mutex &mutex;

    pthread_cond_t cond;
};


#endif //THREADPROXY_CONDVAR_H
