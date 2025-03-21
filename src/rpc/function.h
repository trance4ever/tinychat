#ifndef __FUNCTION_H__
#define __FUNCTION_H__
#include<functional>
#include<string>
#include<string.h>
#include<unordered_map>
#include "../net/socket_stream.h"
#include "../thread.h"

namespace trance {
    // 映射RPC函数
    enum Function {
        __Myadd = 1,
        __Login = 2
    };
    
    std::string Myadd(std::string& data);
    // 返回哈希表
    std::unordered_map<Function, std::function<std::string(std::string&)>> getMap();
    // 登陆
    std::string Login(std::string& data);

    // 聊天应用服务端
    class ChatServer {
    public:
        // 构造析构函数
        ChatServer() { }
        ~ChatServer() { }
        // 全局服务端指针
        static ChatServer* getGlobalChatServer();
        // 放入数据
        void pushSession(std::string& username, SocketStream::ptr session);
        
    private:
        // 锁资源
        Spinlock m_lock;
        // 存储当前在线的用户信息
        std::unordered_map<std::string, SocketStream::ptr> m_sessions;
    };
    
}

#endif