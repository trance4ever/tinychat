#ifndef __RPC_SERVER_H__
#define __RPC_SERVER_H__

#include<vector>
#include<unordered_map>
#include "../io_thread.h"
#include "../reactor.h"
#include "../net/bytearray.h"
#include "../net/socket_stream.h"
#include "trance_protocol.h"
#include "function.h"
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
        void OnRead(ByteArray::ptr ba, int socket);
        // 远程调用完毕，返回处理结果
        void OnWrite(ByteArray::ptr ba, int socket, Response res);
        // 监听可写事件
        void sendResponse(int socket, Response& res);
        // 获取会话
        SocketStream::ptr getSession(int socket);
        // 获得服务器对象
        static RPCServer* getGlobalRPCServer();
    private:
        // 锁资源
        Spinlock m_lock;
        // IO线程组，处理请求
        std::vector<IOThread*> m_iothreads;
        // 主线程，接收请求
        Reactor* m_reactor {nullptr};
        // 主线程套接字
        Socket::ptr m_server {nullptr};
        // 轮训指针
        int threadIdx {0};
        // 存储会话
        std::unordered_map<int, SocketStream::ptr> m_sessions;
        // 存储时间
        std::unordered_map<int, FdEvent*> m_events;
    };

}

#endif