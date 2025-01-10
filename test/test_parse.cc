#include "../src/easyconfig/parse.h"
using namespace easyconfig;


int main() {
    __GETNODE__("../src/easyconfig/config.txt", nodes);
    for(auto lists : nodes) {
        for(auto pair : lists) {
            std::cout << pair.first << " " << pair.second << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}