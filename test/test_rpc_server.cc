#include "../src/rpc/rpc_server.h"
#include "../src/init.h"
using namespace trance;
using namespace std;

int main() {
    RPCServer r;
    r.start();
    return 0;
}