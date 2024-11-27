#include "parse.h"
using namespace easyconfig;


int main() {
    easyconfig::parse p = easyconfig::parse("config.txt");
    std::vector<std::vector<std::pair<std::string, std::string>>> nodes = p.getNode();
    for(auto lists : nodes) {
        for(auto pair : lists) {
            std::cout << pair.first << " " << pair.second << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}