#include "../src/init.h"
#include "../src/log.h"
#include "../src/io_thread.h"
#include<unistd.h>
using namespace trance;
using namespace std;

int main() {
    vector<IOThread*> threads(4);
    for(int i = 0; i < 4; ++i) {
        threads[i] = new IOThread();
    }
    for(int i = 0; i < 4; ++i) {
        auto f = [=]() {
            cout << "iothread[" << i << "] is printing" << endl;
        };
        threads[i]->getReactor()->getTimer()->addTimerEvent(500, f, true);
    }
    INFO_LOG("before start")
    usleep(2000000);
    for(int i = 0; i < 4; ++i) {
        threads[i]->start();
    }
    INFO_LOG("after start")
    usleep(10000000);
    for(int i = 0; i < 4; ++i) {
        threads[i]->stop();
    }
    INFO_LOG("program stop")
    return 0;
}