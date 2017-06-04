#include "Mutex.h"

Mutex::Mutex() {
//    std::cerr << "init mutex " << this << std::endl;
    if (!attr){
        attr = new pthread_mutexattr_t;

        pthread_mutexattr_init(attr);
        pthread_mutexattr_settype(attr, PTHREAD_MUTEX_RECURSIVE);
    }

    pthread_mutex_init(&mutex, attr);
}

Mutex::~Mutex() {
    pthread_mutex_destroy(&mutex);
}

void Mutex::lock() {
    if (pthread_mutex_lock(&mutex) < 0)
        throw std::invalid_argument("unable to lock mutex");
}

void Mutex::unlock(){
    if (pthread_mutex_unlock(&mutex) < 0)
        throw std::invalid_argument("unable to unlock mutex");
}

pthread_mutexattr_t *Mutex::attr = nullptr;