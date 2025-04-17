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
#include "../init.h"

namespace trance {
    // 映射RPC函数
    enum Function {
        __Myadd = 1,
        __Login = 2,
        __AddFriend = 3,
        __Sync = 4,
        __Agree = 5,
        __Refuse = 6,
        __IsOnlie = 7,
        __SendMessage = 8
    };
    
    std::string Myadd(std::string& data);
    // 返回哈希表
    std::unordered_map<Function, std::function<std::string(std::string&)>> getMap();
    // 登陆
    // 请求数据格式: username/password/socketId
    // 响应数据格式: 好友名称[未接收消息数量]/......
    std::string Login(std::string& data);

    // 添加好友
    // 请求数据格式: username/addfriendname
    // 响应数据格式: SUCCESS/ERROR[do not have]
    std::string AddFriend(std::string& data);

    // 同步历史数据
    // 请求数据格式: username
    // 响应数据格式: 文件中累计数据
    std::string Sync(std::string& data);

    // 同意添加好友
    // 请求数据格式: username/friendname
    // 响应数据格式: SUCCESS/ERROR 发起人端: P?username
    std::string Agree(std::string& data);

    // 拒绝添加好友
    // 请求数据格式: username/friendname
    // 响应数据格式: SUCCESS/ERROR 发起人端: R?username
    std::string Refuse(std::string& data);

    // 查询好友是否在线
    // 请求数据格式: username
    // 响应数据格式: SUCCESS[Y/N]
    std::string isOnlie(std::string& data);

    // 聊天
    // 请求数据格式: sender/receiver/message
    // 响应数据格式: SUCCESS
    std::string sendMessage(std::string& data);
    

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
        // 删除数据
        void rmSession(std::string& username);
        // 解析文件
        void parseUserFile();
        // 检验用户账号名密码
        Status exam(std::string& username, std::string& password, int sessionId);
        // 添加好友
        std::string AddFriend(std::string& username, std::string& addedname);
        // 向文件中追加信息
        void append2File(std::string& filename, std::string& str);
        // 同步历史信息
        std::string sync(std::string& username);
        // 读取文件内容
        void readFile(std::string& filename, std::stringstream& ss);
        // 同意添加好友
        std::string agree(std::string& username, std::string& friendname);
        // 拒绝添加好友
        std::string refuse(std::string& username, std::string& friendname);
        // 查询用户是否在线
        bool isOnlie(std::string& username);
        // 发送消息
        std::string sendMessage(std::string& sender, std::string& receiver, std::string& message);
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