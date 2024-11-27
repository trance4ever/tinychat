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
        //解析路径的文件
        std::vector<std::vector<std::pair<std::string, std::string>>> getNode();

    private:
        std::string path;
        std::ifstream fs;
    };
}

#endif