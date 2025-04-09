#include "rpc_server.h"
#include "../log.h"
#include "../init.h"

namespace trance {
    // IO线程个数
    static int g_iothread_num = 7;

    static RPCServer* g_rpcserver = nullptr;

    RPCServer* RPCServer::getGlobalRPCServer() {
        if(g_rpcserver == nullptr) {
            g_rpcserver = new RPCServer();
        }
        return g_rpcserver;
    }

    RPCServer::~RPCServer() {
        for(int i = 0; i < m_iothreads.size(); ++i) {
            delete m_iothreads[i];
        }
        if(m_reactor) {
            delete m_reactor;
        }
    }

    RPCServer::RPCServer() {
        m_iothreads.resize(g_iothread_num);
        m_reactor = new Reactor();
        for(int i = 0; i < m_iothreads.size(); ++i) {
            m_iothreads[i] = new IOThread();
        }
        init();
    }

    void RPCServer::init() {
        m_server = Socket::createIPv4TCP();
        int port = Config::getGlobalConfig()->getConfig<int>("RPCServer::port");
        m_server->bind(port);
        m_server->listen();
    }

    void RPCServer::start() {
        FMT_INFO_LOG("server starting, iothread num: %d", g_iothread_num)
        // 监听必要的事件
        FdEvent* mainEvent = new FdEvent(m_server->getSock());
        mainEvent->listen(std::bind(&RPCServer::OnAccept, this), EPOLLIN);
        m_reactor->addEpollEvent(mainEvent);
        // 开启循环
        for(int i = 0; i < m_iothreads.size(); ++i) {
            m_iothreads[i]->start();
        }
        m_reactor->loop();
    }

    void RPCServer::stop() {
        for(int i = 0; i < m_iothreads.size(); ++i) {
            m_iothreads[i]->stop();
        }
    }

    void RPCServer::OnAccept() {
        Socket::ptr client = m_server->accept();
        int idx = threadIdx++ % m_iothreads.size();
        IOThread* thread = m_iothreads[idx];
        FdEvent* clientEvent = new FdEvent(client->getSock());
        SocketStream::ptr session = std::make_shared<SocketStream>(client);
        auto f = [=]() {
            OnRead(std::make_shared<ByteArray>(), client->getSock(), clientEvent);
        };
        {
            ScopedLock<Spinlock> lock(m_lock);
            m_sessions[client->getSock()] = session;
        }
        clientEvent->listen(f, EPOLLIN);
        clientEvent->setEpollOneShot();
        thread->getReactor()->addEpollEvent(clientEvent);
        FMT_INFO_LOG("new connected %s", client->printInfo().c_str())
    }

    void RPCServer::OnRead(ByteArray::ptr ba, int socket, FdEvent* listenedEvent) {
        ScopedLock<Spinlock> lock(m_lock);
        SocketStream::ptr session = m_sessions[socket];
        lock.unlock();
        int rt = session->read(ba, 2);
        if(rt == 0) {
            FMT_INFO_LOG("client disconnect, info:%s", session->getSocket()->printInfo().c_str())
            Reactor::getCurReactor()->delEpollEvent(session->getSocket()->getSock());
            delete listenedEvent;
            ScopedLock<Spinlock> lock(m_lock);
            std::unordered_map<int, SocketStream::ptr>::iterator it = m_sessions.find(socket);
            if(it != m_sessions.end()) {
                m_sessions.erase(it);
            }
            lock.unlock();
            return;
        }
        uint16_t length;
        ba->readOnly(&length, 2);
        byteswap<uint16_t>()(length);
        rt = session->read(ba, length - 2);
        if(rt == 0) {
            FMT_INFO_LOG("client disconnect, info:%s", session->getSocket()->printInfo().c_str())
            Reactor::getCurReactor()->delEpollEvent(session->getSocket()->getSock());
            delete listenedEvent;
            ScopedLock<Spinlock> lock(m_lock);
            std::unordered_map<int, SocketStream::ptr>::iterator it = m_sessions.find(socket);
            if(it != m_sessions.end()) {
                m_sessions.erase(it);
            }
            lock.unlock();
            return;
        }
        char buf[length];
        ba->read(buf, length);
        Request r(buf);
        FMT_INFO_LOG("get client request, message: %s", r.message.c_str())
        /*
            调用rpc函数，计算响应体
        */
        std::stringstream ss;
        if(r.fun_code == Function::__Login) {
            r.res_data.push_back('/');
            r.res_data.append(std::to_string(socket));
        }
        ss << getMap()[(Function)r.fun_code](r.res_data);
        Response res(1, ss.str(), "OK");
        auto f = [=]() {
            OnWrite(std::make_shared<ByteArray>(), socket, res, listenedEvent);
        };
        listenedEvent->listen(f, EPOLLOUT);
        Reactor::getCurReactor()->addEpollEvent(listenedEvent);
    }

    void RPCServer::OnWrite(ByteArray::ptr ba, int socket, Response res, FdEvent* listenedEvent) {
        ScopedLock<Spinlock> lock(m_lock);
        SocketStream::ptr session = m_sessions[socket];
        lock.unlock();
        uint16_t length = res.size();
        char buf[length];
        res.serialization(buf);
        ba->write(buf, length);
        session->write(ba, length);
        listenedEvent->cancleEvent(EPOLLOUT);
        Reactor::getCurReactor()->addEpollEvent(listenedEvent);
    }

    SocketStream::ptr RPCServer::getSession(int socket) {
        ScopedLock<Spinlock> lock(m_lock);
        std::unordered_map<int, SocketStream::ptr>::iterator it = m_sessions.find(socket);
        if(it != m_sessions.end()) {
            return m_sessions[socket];
        }
        else {
            FMT_ERROR_LOG("can't find session, socket: %d", socket)
        }
    }

}