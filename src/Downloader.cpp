//
// Created by saltuk on 12/22/16.
//

#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include "Downloader.h"

Downloader::Downloader(std::shared_ptr<CacheEntry> entry, CacheManager *cache, Request *request) :
    cache(cache),
    entry(entry),
    request(*request),
    buff(new char[buffSize]){
 //   std::cerr << "start new client " << this << " with buf " << (void*)buff <<std::endl;

    std::cerr << "downloader with request " << request << " starts" << std::endl;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    hostent *serverHost = gethostbyname(request->getHost().c_str());
    if (!serverHost)
        return;

    sockaddr_in	servAddr = {0};
    servAddr.sin_family = AF_INET;
    memcpy(&servAddr.sin_addr.s_addr, serverHost->h_addr_list[0], sizeof(in_addr));
    servAddr.sin_port=htons(80);

    if (connect(fd, (sockaddr*)&servAddr, sizeof(sockaddr_in)) < 0) {
        perror("connecting:");
        return;
    }
    entry->setState(CacheEntry::Loading);
}

void Downloader::run() {
    if (entry->getState() != CacheEntry::Loading)
        return;

    ssize_t ret = write(fd, request.getData().data(), request.getData().size());

    if (ret < 0){
        entry->setState(CacheEntry::Broken);
        return;
    }
    assert(ret == request.getData().size());

    for(;;){
        static const int min_n = 11;
        unsigned long was = entry->getSize();
        ssize_t len = read(fd, buff, buffSize);

    //    std::cerr << "server read " << len << " bytes " << std::endl;
        if (len <= 0){
            if (was < min_n)
                entry->setState(CacheEntry::Broken);
            else
                entry->setState(CacheEntry::Finished);
            return;
        }
        entry->writeData(buff, (size_t)len);

        if (was < min_n && was + len >= min_n){
            const char *p = entry->getP();
            int i;
            for(i = 0; i < min_n - 1 && *p != ' '; ++i, ++p);
            ++p;
            if (i < min_n - 1 && *p != '2')
                cache->detachEntry(request.getHost()+request.getRequest());
        }
    }

}

Downloader::~Downloader() {
    delete [] buff;
    close(fd);
    if (entry->getState() != CacheEntry::Finished){
        entry->setState(CacheEntry::Broken);
        cache->detachEntry(request.getHost()+request.getRequest());
    }
}
