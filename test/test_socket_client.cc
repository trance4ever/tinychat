#include "../src/init.h"
#include "../src/log.h"
#include "../src/net/socket.h"
#include "../src/net/socket_stream.h"
#include "../src/util.h"
#include "../src/rpc/trance_protocol.h"
using namespace trance;
using namespace std;

int main() {
    Socket::ptr client = Socket::createIPv4TCP();
    client->connect(IPv4Address::create("127.0.0.1:7400"));
    SocketStream::ptr ss_client(new SocketStream(client));
    ByteArray::ptr ba(new ByteArray());
    while(1) {
        sleep(1000);
        // 发送请求体
        uint64_t para = 232834234223;
        Request r(1, 4, &para, string("hello!"));
        uint16_t length = r.size();
        char buf[length];
        r.serialization(buf);
        ba->write(buf, length);
        ss_client->write(ba, length);
        FMT_INFO_LOG("send request, data length: %d", length)

        // ba->write(data, 13);
        // ba->writeDouble(1.234567);
        // FMT_INFO_LOG("send data: %s, %f", data, 1.234567)
    }
    return 0;
}