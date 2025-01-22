#ifndef __THREAD_H__
#define __THREAD_H__
#include<pthread.h>
#include<semaphore.h>
#include<vector>
#include<functional>
#include<memory>
#include<iostream>
#include "util.h"

namespace trance {
    // 继承该类禁止拷贝
    class DisableCopy {
    public:
        // 默认无参构造
        DisableCopy() = default;
        ~DisableCopy() = default;
        // 删除拷贝构造函数
        DisableCopy(const DisableCopy&) = delete;
        // 删除转移构造函数
        DisableCopy(const DisableCopy&&) = delete;
        // 删除=重载
        DisableCopy operator=(const DisableCopy&) = delete;
    };

    // 信号量类
    class Semaphore : public DisableCopy {
    public:
        // 信号量初始化
        Semaphore(uint32_t cnt = 0) {
            sem_init(&m_sem, 0, cnt);
        }
        ~Semaphore() {
            sem_destroy(&m_sem);
        }
        // 等待
        void wait() {
            sem_wait(&m_sem);
        }
        // 唤醒
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
    // 互斥锁
    class Mutex : public DisableCopy {
    public:
        // 初始化锁
        Mutex() { 
            pthread_mutex_init(&m_mutex, nullptr);
        }
        // 析构销毁锁
        ~Mutex() { 
            pthread_mutex_destroy(&m_mutex);
        }
        // 上锁
        void lock() { 
            pthread_mutex_lock(&m_mutex);
        }
        // 解锁
        void unLock() { 
            pthread_mutex_unlock(&m_mutex);
        }
    private:
        // 锁资源
        pthread_mutex_t m_mutex;
    };
    // 读写锁
    class RWMutex : public DisableCopy {
    public:
        // 初始化锁
        RWMutex() { 
            pthread_rwlock_init(&m_lock, nullptr);
        }
        // 销毁锁
        ~RWMutex() {
            pthread_rwlock_destroy(&m_lock);
        }
        // 读者加锁
        void rLock() { 
            pthread_rwlock_rdlock(&m_lock);
        }
        // 写者加锁
        void wLock() { 
            pthread_rwlock_wrlock(&m_lock);
        }
        // 解锁
        void unlock() { 
            pthread_rwlock_unlock(&m_lock);
        }
    private:
        // 锁资源
        pthread_rwlock_t m_lock;
    };
    //自旋锁
	class Spinlock : public DisableCopy {
	public:
        // 初始化
        Spinlock() {
            pthread_spin_init(&m_mutex, 0);
        }
        // 销毁锁
	    ~Spinlock() {
	  	    pthread_spin_destroy(&m_mutex);
	    }
        // 上锁
	    void lock() {
	  	    pthread_spin_lock(&m_mutex);
	    }
        // 解锁
		void unlock() {
			pthread_spin_unlock(&m_mutex);
		} 
	private:
        // 锁资源
		pthread_spinlock_t m_mutex;
	};
    // 满足RAII的锁
    template<typename T>
    class ScopedLock : public DisableCopy {
    public:
        // 删除无参构造，必须拷贝构造
        ScopedLock() = delete;
        // 拷贝构造，获取资源即上锁
        ScopedLock(T& mutex) : m_mutex(mutex) {
            m_mutex.lock();
        }
        // 析构函数，销毁即解锁
        ~ScopedLock() { m_mutex.unlock();}
        // 手动上锁
        void lock() { m_mutex.lock();}
        // 手动解锁
        void unlock() { m_mutex.unlock();}
    private:
        // 引用锁资源
        T& m_mutex;
    };
}

#endif