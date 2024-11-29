#ifndef __PARSE_H__
#define __PARSE_H__
#include<string>
#include<vector>
#include<iostream>
#include<fstream>

namespace easyconfig {
    class parse{
    public:
        parse(std::string str);
        ~parse();
        //解析路径的文件
        std::vector<std::vector<std::pair<std::string, std::string>>> getNode();
    private:
        std::string path;
        std::ifstream fs;
    };
    //获得指定路径的节点到指定容器
    #define __GETNODE__(str, vec) \
    std::vector<std::vector<std::pair<std::string, std::string>>> vec = parse(str).getNode();
}

#endif