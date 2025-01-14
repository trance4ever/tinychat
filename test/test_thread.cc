#include "../src/thread.h"
#include "../src/init.h"
using namespace trance;

int main() {
    auto f = []() {
        std::cout << "hello world!" << std::endl;
    };
    std::vector<Thread::ptr> threads(10);
    for(int i = 0; i < threads.size(); ++i) {
        threads[i] = std::make_shared<Thread>(f);
    }
    for(int i = 0; i < threads.size(); ++i) {
        threads[i]->join();
    }
    return 0;
}