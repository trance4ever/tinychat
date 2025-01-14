#include "thread.h"

namespace trance {
    static thread_local Thread* t_thread = nullptr;
    
    Thread::Thread(std::function<void()> cb, std::string name) 
        : m_cb(cb),
        m_name(name),
        m_id(getPid()) {
        int rt = pthread_create(&m_threadId, nullptr, &Thread::runCb, this);
        if(rt) {
            FMT_ERROR_LOG("failed to create thread, rt = %d", rt);
            throw std::logic_error("pthread_create error");
        }
        m_sem.wait();
    }

    Thread* Thread::getCurThread() {
        return t_thread;
    }

    void* Thread::runCb(void* arg) {
        Thread* thread = (Thread*)arg;
        t_thread = thread;
        std::function<void()> cb;
        cb.swap(thread->m_cb);
        thread->m_sem.notify();
        thread->m_id = getThreadId();
        std::cout << "thread: " << thread->m_id << " before callback" << std::endl;
        cb();
        std::cout << "thread: " << thread->m_id << " after callback" << std::endl;
        return 0;
    }

    void Thread::join() {
        if(m_threadId) {
            int rt = pthread_join(m_threadId, nullptr);
            if(rt) {
                FMT_ERROR_LOG("failed to destory thread, rt = %d", rt)
                throw std::logic_error("pthread_join error");
            }
            m_threadId = 0;
        }
    }
}