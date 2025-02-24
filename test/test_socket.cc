#include "../src/init.h"
#include "../src/log.h"
#include "../src/net/socket.h"
#include "../src/net/socket_stream.h"
#include "../src/util.h"
using namespace trance;
using namespace std;

int main() {

    Socket::ptr server = Socket::createIPv4TCP();
    server->bind(7072);
    server->listen();
    Socket::ptr session = server->accept();
    SocketStream::ptr ss_session(new SocketStream(session));
    ByteArray::ptr ba(new ByteArray());
    while(1) {
        sleep(500);
        int rt = ss_session->read(ba, 13);
        if(rt == 0) {
            INFO_LOG("client close")
            return 0;
        }
        char buf[rt];
        ba->read(buf, rt);
        cout << buf << endl;
    }

    return 0;
}