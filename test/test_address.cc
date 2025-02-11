#include "../src/net/address.h"
#include "src/init.h"
#include "src/log.h"

using namespace trance;
using namespace std;

int main() {
    Address::ptr p1 = IPv4Address::create("192.168.0.1:22");
    Address::ptr p2 = IPv4Address::create("192.168.0.1", 3306);
    Address::ptr p3 = IPv4Address::create("192.168.0.1234", 114514);
    cout << p1->isValid() << endl;
    cout << p2->isValid() << endl;
    cout << p3->isValid() << endl;
    p3->setPort(3389);
    cout << p3->isValid() << endl;
    p3->setIp("192.168.1.116");
    p1->setIp("192.168.0.1234");
    cout << p1->isValid() << endl;
    cout << p2->isValid() << endl;
    p3->setPort(234222);
    cout << p3->isValid() << endl;
    INFO_LOG(p2->toString().c_str())
    return 0;
}