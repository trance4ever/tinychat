#include "parse.h"

namespace easyconfig {
    parse::parse(std::string str) {
        this->path = str;
        fs.open(path);
        if(!fs.is_open()) {
            std::cout << "failed to load path" << std::endl;
        }
    }

    std::vector<std::vector<std::pair<std::string, std::string>>> parse::getNode() {
        std::string tmp, val;
        std::vector<std::vector<std::pair<std::string, std::string>>> ans;
        std::string prefix;
        int index = -1, cnt = 0;
        while(fs >> tmp) {
            if(tmp[0] == '-') {
                ++cnt;
                fs >> prefix;
                ans.resize(ans.size() + 1);
                ++index;
            }
            else {
                tmp.pop_back();
                fs >> val;
                ans[index].push_back({prefix + "::" + tmp, val});
            }
        }
        return ans;
    }

    parse::~parse() {
        fs.close();
    }
}