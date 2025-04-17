#ifndef __CHATCLIENT_H__
#define __CHATCLIENT_H__
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<string>
#include<queue>
#include "../rpc/trance_protocol.h"
#include "../log.h"
#include "../net/socket_stream.h"
#include "../thread.h"
#include "../rpc/function.h"
#include "../util.h"

namespace trance {

    class ChatClient {
    public:
        // 构造析构函数
        ChatClient();
        ~ChatClient();
        // 启动
        void start();
        // 子线程
        static void* Main(void* arg);
        // 循环接收
        void circularRecv();
        // 处理响应体
        void handleResponse(std::string& data);
        // 登陆界面
        void loginUI();
        // 好友界面
        void friendsUI();
        // 发送请求体到服务端
        void sendRequest(Function func, std::string& data, std::string& message);
        // 同步操作
        void sync();
        // 到达聊天界面
        void interface2();
        // 处理请求
        void handleRequest(std::string& data);
        // 接收响应体
        Response recvResponse();
        // 处理好友请求界面
        void quiryUI(std::string& friendname);
        // 信息页面
        void printInfo(std::string& data);
        // 检查是否为数字
        bool isNum(std::string& choice);
        // 好友聊天页面
        void interface3(int idx);
        // 好友聊天UI
        void chatUI(int idx);
    private:
        // 存储聊天信息
        std::map<std::string, std::vector<std::string>> m_messages;
        // 存储好友信息
        std::vector<std::string> m_friends;
        // 存储请求信息
        std::queue<std::string> m_requests;
        // 保存会话
        SocketStream::ptr m_session {nullptr};
        // 数据缓冲区
        ByteArray::ptr m_buffer {nullptr};
        // 信号量
        Semaphore m_sem;
        // 锁资源
        Spinlock m_lock;
        // 子线程标志
        pthread_t subThread;
        // 启动信号量
        Semaphore m_startSem;
        // 当前页面
        int m_interface {1};
        // 当前用户
        std::string m_username;
        // 聊天用户id
        int m_friendId {-1};
    };

}

#endif