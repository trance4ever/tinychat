#include<iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include "../src/init.h"
#include "../src/log.h"
#include "../src/reactor.h"
#include "../src/fd_event.h"
#include "../src/thread.h"
using namespace trance;

int main() {
    int p[2];
    if(pipe(p) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    auto f1 = []() {
        std::cout << "write event trigger" << std::endl;
    };

    auto f2 = [=]() {
        char buffer[100];
        read(p[0], buffer, sizeof(buffer));
        std::cout << "read event trigger" << std::endl;
        std::cout << "info: " << buffer << std::endl;
    };
    auto f3 = [=]() {
        const char *msg = "Hello, Pipe!";
        while(1) {
            write(p[1], msg, strlen(msg)); 
            usleep(500000);  
        }
    };
    Thread t(f3);

    FdEvent fe(p[0]);
    fe.listen(f2, EPOLLIN);

    FdEvent fe2(p[1]);
    fe2.listen(f1, EPOLLOUT);

    Reactor::ptr r = std::make_shared<Reactor>();
    r->addEpollEvent(&fe);
    r->addEpollEvent(&fe2);
    r->loop();
    return 0;
}