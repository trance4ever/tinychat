#ifndef __SERVER_H__
#define __SERVER_H__
#include<vector>
#include<algorithm>
#include<map>
#include<functional>
#include<iostream>
#include<queue>
#include<atomic>
#include "thread.h"

namespace trance {
    // 线程池类
    class Server {
    public:
        // 构造函数，指定线程池大小
        Server(int threads);
        // 启动线程池
        void start();
        // 停止线程池
        void stop();
        // 添加任务
        void addTask();
        // 删除任务
        void delTask();
    private:
        // 锁资源
        Spinlock m_lock;
        // 任务队列
        std::queue<std::function<void()>> m_cbs;
        // 线程池
        std::vector<Thread::ptr> m_threads;
        // 空闲线程数量
        std::atomic<int> m_idleThread = {0};
        // 活跃线程数量
        std::atomic<int> m_activeThread = {0};
        // 线程总数
        int m_totalThreads;
    };
}

#endif