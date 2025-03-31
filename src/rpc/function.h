#ifndef __FUNCTION_H__
#define __FUNCTION_H__
#include<functional>
#include<string>
#include<string.h>
#include<fstream>
#include<unordered_map>
#include "../net/socket_stream.h"
#include "../thread.h"
#include "../rpc/trance_protocol.h"

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
    // 请求数据格式: username/password/socketId
    // 响应数据格式: 好友名称[未接收消息数量]/......
    std::string Login(std::string& data);

    // 聊天应用服务端
    class ChatServer {
    public:
        // 构造析构函数
        ChatServer();
        ~ChatServer() { }
        // 全局服务端指针
        static ChatServer* getGlobalChatServer();
        // 放入数据
        void pushSession(std::string& username, int sessionId);
        // 解析文件
        void parseUserFile();
        // 检验用户账号名密码
        Status exam(std::string& username, std::string& password, int sessionId);
    private:
        // 锁资源
        Spinlock m_lock;
        // 存储当前在线的用户信息
        std::unordered_map<std::string, int> m_sessions;
        // 存储用户账号密码信息
        std::unordered_map<std::string, std::string> m_userInfos;
    };
    
}

#endif