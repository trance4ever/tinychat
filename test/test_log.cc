#include "../src/log.h"
using namespace trance;

int main() {
    int size = 5;
    for(int i = 0; i < 100000; ++i) {
        FMT_INFO_LOG("test log line%d", i)
    }
    return 0;
}