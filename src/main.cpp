#include <csignal>
#include <cstdlib>
#include "HTTPProxy.h"

bool working;

void onExit(int){
    std::cerr << "exit" << std::endl;
    working = false;
}


int main(int argc, char **argv) {
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, onExit);

    int port = 4242;

    if (argc > 1) {
        port = atoi(argv[1]);
    }

    working = true;

    HTTPProxy proxy(port, working);
    proxy.run();

}
