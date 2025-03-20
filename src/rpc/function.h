#ifndef __FUNCTION_H__
#define __FUNCTION_H__
#include<functional>
#include<string>
#include<string.h>
#include<unordered_map>

namespace trance {

    enum Function {
        __Myadd = 1
    };

    std::string Myadd(std::string& data);

    std::unordered_map<Function, std::function<std::string(std::string&)>> getMap();
}

#endif