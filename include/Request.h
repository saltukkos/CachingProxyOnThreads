//
// Created by saltuk on 12/22/16.
//

#ifndef THREADPROXY_REQUEST_H
#define THREADPROXY_REQUEST_H


#include <vector>
#include <string>

class Request {
public:
    Request();
    bool putData(const char *buff, size_t from, size_t len);
    const std::vector<char> &getData();
    std::string getHost();
    std::string getRequest();
    bool isKeepAlive();

private:
    const std::vector<char> requestEnd = {'\r', '\n', '\r', '\n'};

    bool ready;
    bool keepAlive;
    std::vector<char> data;
    std::string host;
    std::string request;

    bool isEnd();
    void parseRequest();
};


#endif //THREADPROXY_REQUEST_H
