#ifndef __IOTHREAD_H__
#define __IOTHREAD_H__
#include<pthread.h>
#include "reactor.h"
#include "thread.h"

namespace trance {

    class IOThread {
    public:
        // 构造函数
        IOThread();
        // 析构函数
        ~IOThread();
        // 得到Reactor实例
        Reactor* getReactor() { return m_reactor;}
        static void* Main(void* arg);
        // 启动循环
        void start();
        // 终止循环
        void stop();
    private:
        // 该线程拥有的Reactor实例
        Reactor* m_reactor = nullptr;
        // 线程标记
        pthread_t m_threadIcon;
        // 初始化信号量信号量
        Semaphore m_initSem;
        // 启动信号量
        Semaphore m_startSem;
    };

}

#endif