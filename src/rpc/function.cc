#include "function.h"
#include "../easyconfig/config.h"

namespace trance {

    static std::unordered_map<Function, std::function<std::string(std::string&)>> 
            m_global_function_maps = {
                {Function::__Myadd, Myadd},
                {Function::__Login, Login}
            };

    static ChatServer* g_chatServer = nullptr;

    std::unordered_map<Function, std::function<std::string(std::string&)>> getMap() {
        return m_global_function_maps;
    }

    std::string Myadd(std::string& data) {
        int a = std::stoi(data.substr(0, 2));
        int b = std::stoi(data.substr(2, 2));
        return std::to_string(a + b);
    }

    ChatServer* ChatServer::getGlobalChatServer() {
        if(g_chatServer == nullptr) {
            g_chatServer = new ChatServer();
        }
        return g_chatServer;
    }

    std::string Login(std::string& data) {
        
    }

    void ChatServer::pushSession(std::string& username, SocketStream::ptr session) {
        ScopedLock<Spinlock> lock(m_lock);
        m_sessions[username] = session;
    }

}