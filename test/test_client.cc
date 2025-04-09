#include "../src/init.h"
#include "../src/log.h"
#include "../src/client/chat_client.h"
using namespace trance;
using namespace std;
int main() {
    // 启动客户端
    ChatClient cc;
    cc.start();
    return 0;
}