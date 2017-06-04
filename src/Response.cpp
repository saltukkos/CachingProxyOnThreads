//
// Created by saltuk on 12/22/16.
//

#include <sstream>
#include "Response.h"

/*bool Response::put(char sym) {
    data.push_back(sym);
    unsigned long size = data.size();
    if (size >= 4 && data[size - 4] == '\r' && data[size - 3] == '\n'
            && data[size - 2] == '\r' && data[size - 1] == '\n'){
        if (isKeepAlive)
            parseRequest();
        return true;
    }
}

void pushStringToVector(std::string str, std::vector<char> &vector){
    for (char c : str){
        vector.push_back(c);
    }
    vector.push_back('\n');
}

void Response::parseRequest() {
    data.push_back(0);
    bool connectSet = false;
    std::istringstream resp(data.data());
    std::string temp;
    std::vector<char> parsedRequest;

    while (std::getline(resp, temp) && temp != "\r"){
        if (temp.find("Connection:") == 0){
            connectSet = true;
            pushStringToVector("Connection: keep-alive\r", parsedRequest);
            continue;
        }
        pushStringToVector(temp, parsedRequest);

    }
    if (!connectSet)
        pushStringToVector("Connection: keep-alive\r", parsedRequest);

    parsedRequest.push_back('\r');
    parsedRequest.push_back('\n');

    data = parsedRequest;
}

std::vector<char> &Response::getData() {
    return data;
}

Response::Response(bool isKeepAlive) :
    isKeepAlive(isKeepAlive) {}
*/