#ifndef __CHATCLIENT_H__
#define __CHATCLIENT_H__
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include "../rpc/trance_protocol.h"
#include "../log.h"
#include "../net/socket_stream.h"
#include "../thread.h"
#include "../rpc/function.h"

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
        // 发送请求体到服务端
        void sendRequest(Function func, std::string& data, std::string& message);
    private:
        // 存储聊天信息
        std::map<std::string, std::vector<std::string>> m_messages;
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
    };

}

#endif