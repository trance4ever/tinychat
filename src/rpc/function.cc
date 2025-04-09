#include "function.h"
#include "../easyconfig/config.h"

namespace trance {

    static std::unordered_map<Function, std::function<std::string(std::string&)>> 
            m_global_function_maps = {
                {Function::__Myadd, Myadd},
                {Function::__Login, Login}
            };

    static ChatServer* g_chatServer = nullptr;

    ChatServer::ChatServer() {
        parseUserFile();
    }

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
        ChatServer* chatServer = ChatServer::getGlobalChatServer();
        int split_idx = data.find_first_of('/'), sock_idx = data.find_last_of('/');
        if(split_idx == data.size()) {
            ERROR_LOG("Login() error, data format error")
            return "ERROR";
        }
        std::string username = data.substr(0, split_idx);
        std::string password = data.substr(split_idx + 1, sock_idx - split_idx - 1);
        std::string str_socket = data.substr(sock_idx + 1);
        int sessionId = std::stoi(str_socket);
        /*
            查询数据，先用文本文档模拟数据库，后续基于MySQL实现连接池
        */
        Status s = chatServer->exam(username, password, sessionId);
        if(s == SUCCESS) {
            return "SUCCESS";
        }
        else {
            return "ERROR";
        }
    }

    void ChatServer::pushSession(std::string& username, int sessionId) {
        ScopedLock<Spinlock> lock(m_lock);
        m_sessions[username] = sessionId;
    }

    void ChatServer::parseUserFile() {
        std::ifstream ifs;
        ifs.open("../chat/server/users.txt");
        if(!ifs.is_open()) {
            ERROR_LOG("ChatServer::parseUserFile() error, file open error")
            ifs.close();
            return;
        }
        std::string tmp;
        while(ifs >> tmp) {
            int split_idx = tmp.find_first_of(':');
            m_userInfos[tmp.substr(0, split_idx)] = tmp.substr(split_idx + 1);
        }
        ifs.close();
    }

    Status ChatServer::exam(std::string& username, std::string& password, int sessionId) {
        auto it = m_userInfos.find(username);
        if(it == m_userInfos.end()) {
            return Status::USERNAME_ERROR;
        }
        else if(password != m_userInfos[username]) {
            return Status::PASSWORD_ERROR;
        }
        pushSession(username, sessionId);
        return Status::SUCCESS;
    }

    void ChatServer::rmSession(std::string& username) {
        ScopedLock<Spinlock> lock(m_lock);
        auto it = m_sessions.find(username);
        if(it != m_sessions.end()) {
            m_sessions.erase(it);
        }
        else {
            ERROR_LOG("ChatServer::rmSession() error, username not found")
        }
    }

}