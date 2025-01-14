#ifndef __THREAD_H__
#define __THREAD_H__
#include<pthread.h>
#include<semaphore.h>
#include<vector>
#include<functional>
#include<memory>
#include<iostream>
#include "log.h"
#include "util.h"

namespace trance {
    // 继承该类禁止拷贝
    class DisableCopy {
    public:
        DisableCopy() = default;
        ~DisableCopy() = default;
        DisableCopy(const DisableCopy&) = delete;
        DisableCopy(const DisableCopy&&) = delete;
        DisableCopy operator=(const DisableCopy&) = delete;
    };

    // 信号量类
    class Semaphore : public DisableCopy {
    public:
        Semaphore(uint32_t cnt = 0) {
            sem_init(&m_sem, 0, cnt);
        }
        ~Semaphore() {
            sem_destroy(&m_sem);
        }
        void wait() {
            sem_wait(&m_sem);
        }
        void notify() {
            sem_post(&m_sem);
        }
    private:
        sem_t m_sem;
    };

    // 线程类
    class Thread {
    public:
        typedef std::shared_ptr<Thread> ptr;
        // 构造函数
        Thread(std::function<void()> cb, std::string name = "UNKONW");
        // 线程任务函数
        static void* runCb(void* arg);
        // 获得当前线程对象指针
        static Thread* getCurThread();
        // 获得进程号
        pid_t getPid() { return m_id;}
        // 销毁线程
        void join();
    private:
        // 回调函数
        std::function<void()> m_cb;
        // 线程名称
        std::string m_name;
        // 线程号
        pthread_t m_id;
        // 线程号
        pthread_t m_threadId;
        // 信号量
        Semaphore m_sem;
    };

}

#endif