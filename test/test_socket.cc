#include "../src/init.h"
#include "../src/log.h"
#include "../src/net/socket.h"
#include "../src/util.h"
using namespace trance;
using namespace std;

int main() {

    Socket::ptr server = Socket::createIPv4TCP();
    server->bind(7072);
    server->listen();
    Socket::ptr session = server->accept();
    while(1) {
        sleep(500);
        char buf[13];
        int rt = session->recv(buf, 13);
        if(rt == 0) {
            INFO_LOG("client close")
            return 0;
        }
        cout << buf << endl;
    }

    return 0;
}