#include "function.h"
#include "../easyconfig/config.h"

namespace trance {

    static std::unordered_map<Function, std::function<std::string(std::string&)>> 
            m_global_function_maps = {
                {Function::__Myadd, Myadd}
            };

    std::unordered_map<Function, std::function<std::string(std::string&)>> getMap() {
        return m_global_function_maps;
    }

    std::string Myadd(std::string& data) {
        int a = std::stoi(data.substr(0, 2));
        int b = std::stoi(data.substr(2, 2));
        return std::to_string(a + b);
    }

}