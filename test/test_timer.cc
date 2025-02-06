#include<iostream>
#include "../src/init.h"
#include "../src/log.h"
#include "../src/reactor.h"
#include "../src/timer.h"
#include "../src/fd_event.h"
using namespace trance;

int main() {
    auto f1 = []() {
        std::cout << "loop timer event" << std::endl;
    };

    auto f2 = []() {
        std::cout << "once timer event" << std::endl;
    };

    Reactor::ptr r = std::make_shared<Reactor>(true);
    r->getTimer()->addTimerEvent(1000, f1, true);
    r->getTimer()->addTimerEvent(2000, f2);
    r->loop();
    return 0;
}