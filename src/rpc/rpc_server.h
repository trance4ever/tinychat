#ifndef __RPC_SERVER_H__
#define __RPC_SERVER_H__

#include<vector>
#include "../io_thread.h"
#include "../reactor.h"
#include "../net/bytearray.h"
#include "../net/socket_stream.h"
#include "trance_protocol.h"
#include "../fd_event.h"

namespace trance {
    // RPC服务端
    class RPCServer {
    public:
        // 构造析构函数
        RPCServer();
        ~RPCServer();
        // 初始化操作
        void init();
        // 启动
        void start();
        // 停止, 将销毁IO线程
        void stop();
        // 处理连接请求
        void OnAccept();
        // 接收RPC请求，解析并处理请求
        void OnRead(ByteArray::ptr ba, ByteArray::ptr sba, SocketStream::ptr session);
        // 远程调用完毕，返回处理结果
        void OnWrite(std::weak_ptr<ByteArray> wba, std::weak_ptr<SocketStream> wsession, Response& res);
    private:
        // IO线程组，处理请求
        std::vector<IOThread*> m_iothreads;
        // 主线程，接收请求
        Reactor* m_reactor {nullptr};
        // 主线程套接字
        Socket::ptr m_server {nullptr};
        // 轮训指针
        int threadIdx {0};
    };

}

#endif