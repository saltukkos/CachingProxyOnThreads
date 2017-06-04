//
// Created by saltuk on 12/22/16.
//

#include "CondVar.h"

void CondVar::notifyAll() {
    int err = pthread_cond_broadcast(&cond);
    if (err != 0)
        throw std::invalid_argument("error while broadcast" + err);
}

void CondVar::notify() {
    int err = pthread_cond_signal(&cond);
    if (err != 0)
        throw std::invalid_argument("error while signal" + err);
}

void CondVar::wait() {
    int err = pthread_cond_wait(&cond, &mutex.mutex);
    if (err != 0)
        throw std::invalid_argument("error on wait " + err);
}

CondVar::~CondVar() {
    pthread_cond_destroy(&cond);
}

CondVar::CondVar(Mutex &mutex) :
        mutex(mutex),
        cond(PTHREAD_COND_INITIALIZER) {
 //   std::cerr << "use mutex in cond " << &mutex << std::endl;
}
