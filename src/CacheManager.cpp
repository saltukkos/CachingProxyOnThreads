//
// Created by saltuk on 12/15/16.
//

#include <vector>
#include <algorithm>
#include "CacheManager.h"
#include "MutexGuard.h"

double CacheManager::garbageLimit = 0.7;

void CacheManager::detachEntry(const key_t &key) {
    MutexGuard guard(mutex);

    map.erase(key);
}

CacheManager::val_t CacheManager::getEntry(const key_t &key) {
    MutexGuard guard(mutex);

    CacheManager::val_t &temp = map[key];
    if (!temp.operator bool()) {
        temp.reset(createNode());
    }

    temp->setLastUse(lastUse++);
    return temp;
}

CacheEntry *CacheManager::createNode() {
    MutexGuard guard(mutex);

    if (currentSize.get() >= maxSize){
        typedef std::pair<key_t, val_t> MapEntry;
        std::vector<MapEntry> vector;

        std::cerr << "garbage collector starts" << std::endl;

        for (auto const& it : map){
            if (it.second)
                vector.push_back(it);
        }

        std::sort(vector.begin(), vector.end(),
                  [](const MapEntry &left, const MapEntry &right){
                      return left.second->getLastUse() < right.second->getLastUse();
                  });

        for (auto& it : vector){
            std::cerr << "entry for " << it.first <<" : client count = " << it.second->clientsCount() << " " << std::endl;
            if (it.second->clientsCount() == 0){
                map.erase(it.first);

                if (currentSize.get() < maxSize*garbageLimit) {
                    std::cerr << "garbage collector GG" << std::endl;
                    break;
                }
            }
        }
    }
    if (currentSize.get() >= maxSize)
        throw std::bad_alloc();
    return new CacheEntry(&currentSize);
}

CacheManager::CacheManager() :
        lastUse(0) {}
