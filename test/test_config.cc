#include "../src/easyconfig/config.h"
#include<iostream>
using namespace std;

int main() {
    string str = "114514";
    int num = 114514;
    cout << "fromstr(114514) : " << trance::FromString<int>()(str) << endl;
    cout << "tostring(114514) : " << trance::ToString<int>()(num) << endl; 
    return 0;
}