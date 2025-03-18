#include "../src/init.h"
#include "../src/log.h"
#include "../src/net/socket.h"
#include "../src/net/socket_stream.h"
#include "../src/util.h"
#include "../src/rpc/trance_protocol.h"
using namespace trance;
using namespace std;

int main() {

    Socket::ptr server = Socket::createIPv4TCP();
    server->bind(7400);
    server->listen();
    Socket::ptr session = server->accept();
    SocketStream::ptr ss_session(new SocketStream(session));
    ByteArray::ptr ba(new ByteArray());
    while(1) {
        sleep(500);
        // // 发送响应体
        // uint64_t para = 232834234223;
        // Response r(1, 4, &para, string("nice 2 meet u!"));
        // uint16_t length = r.size();
        // char buf[length];
        // r.serialization(buf);
        // ba->write(buf, length);
        // ss_session->write(ba, length);

        int rt = ss_session->read(ba, 2);
        uint16_t length;
        ba->readOnly(&length, 2);
        byteswap<uint16_t>()(length);
        char buf[length];
        ss_session->read(ba, length - 2);
        ba->read(buf, length);
        Request r(buf);
        FMT_INFO_LOG("recv request body from client, message: %s", r.message.c_str())
        if(rt == 0) {
            INFO_LOG("client close")
            return 0;
        }
        // char buf[13];
        // ba->read(buf, 13);
        // double d = ba->readDouble();
        // cout << buf << " " << d << endl;
    }

    return 0;
}