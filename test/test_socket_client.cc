#include "../src/init.h"
#include "../src/log.h"
#include "../src/net/socket.h"
#include "../src/net/socket_stream.h"
#include "../src/util.h"
using namespace trance;
using namespace std;

int main() {
    Socket::ptr client = Socket::createIPv4TCP();
    client->connect(IPv4Address::create("127.0.0.1:7072"));
    SocketStream::ptr ss_client(new SocketStream(client));
    ByteArray::ptr ba(new ByteArray());
    const char* data = "hello world!";
    while(1) {
        sleep(500);
        FMT_INFO_LOG("send data: %s", data)
        ba->write(data, 13);
        ss_client->write(ba, 13);
    }
    return 0;
}