#include "../src/init.h"
#include "../src/log.h"
#include "../src/net/socket.h"
#include "../src/net/socket_stream.h"
#include "../src/util.h"
#include "../src/rpc/trance_protocol.h"
#include "../src/rpc/function.h"
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
        Request r(Function::__Myadd, string("1234"), string("add"));
        uint16_t length = r.size();
        char buf[length];
        r.serialization(buf);
        ba->write(buf, length);
        ss_client->write(ba, length);
        FMT_INFO_LOG("send request, data length: %d", length)

        int rt = ss_client->read(ba, 2);
        if(rt == 0) {
            INFO_LOG("disconnected ...")
            return 0;
        }
        ba->readOnly(&length, 2);
        byteswap<uint16_t>()(length);
        rt = ss_client->read(ba, length - 2);
        if(rt == 0) {
            INFO_LOG("disconnected ...")
            return 0;
        }
        char buf2[length];
        ba->read(buf2, length);
        Response res(buf2);
        FMT_INFO_LOG("accept response, mesage: %s, result: %s", res.message.c_str(), res.rsp_data.c_str())
    } 
    return 0;
}