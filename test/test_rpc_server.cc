#include "../src/rpc/rpc_server.h"
#include "../src/init.h"
using namespace trance;
using namespace std;

int main() {
    RPCServer* r = RPCServer::getGlobalRPCServer();
    r->start();
    // delete r;
    return 0;
}