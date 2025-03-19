#include "function.h"
#include "../easyconfig/config.h"

namespace trance {

    int Myadd(std::string& data) {
        int a = std::stoi(data.substr(0, 2));
        int b = std::stoi(data.substr(2, 2));
        return a + b;
    }

}