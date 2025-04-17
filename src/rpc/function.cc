#include "function.h"
#include "../easyconfig/config.h"
#include "rpc_server.h"
#include "../util.h"

namespace trance {

    static std::unordered_map<Function, std::function<std::string(std::string&)>> 
            m_global_function_maps = {
                {Function::__Myadd, Myadd},
                {Function::__Login, Login},
                {Function::__AddFriend, AddFriend},
                {Function::__Sync, Sync},
                {Function::__Agree, Agree},
                {Function::__Refuse, Refuse},
                {Function::__IsOnlie, isOnlie},
                {Function::__SendMessage, sendMessage}
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

    std::string isOnlie(std::string& data) {
        ChatServer* chatServer = ChatServer::getGlobalChatServer();
        if(chatServer->isOnlie(data)) {
            return "SUCCESS[Y]";
        }
        else {
            return "SUCCESS[N]";
        }
    }

    std::string Sync(std::string& data) {
        ChatServer* chatServer = ChatServer::getGlobalChatServer();
        return chatServer->sync(data);
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

    std::string AddFriend(std::string& data) {
        int split_idx = data.find_first_of('/');
        std::string username = data.substr(0, split_idx);
        std::string addedname = data.substr(split_idx + 1);
        ChatServer* s = ChatServer::getGlobalChatServer();
        // 交给服务层处理
        return s->AddFriend(username, addedname);
    }
    
    std::string ChatServer::AddFriend(std::string& username, std::string& addedname) {
        // 检查是否有此id
        auto it = m_userInfos.find(addedname);
        int flag = 1;
        // 没有返回错误信息
        if(it == m_userInfos.end()) {
            return "ERROR[do not have such user]";
        }
        else {
            // 目标用户存在，查找目标用户的会话信息
            RPCServer* rpcserver = RPCServer::getGlobalRPCServer();
            auto it = m_sessions.find(addedname);
            std::string info = "A?" + username;
            if(it != m_sessions.end()) {
                SocketStream::ptr session = rpcserver->getSession(it->second);
                // 用户在线则发送即使消息，用户不在线，删除保存的在线用户信息，并且保存到文件
                if(session != nullptr) {
                    flag = 0;
                    Response res(1, info, "OK");
                    rpcserver->sendResponse(it->second, res);
                }
                else {
                    ScopedLock<Spinlock> lock(m_lock);
                    m_sessions.erase(it);
                }
            }
        }
        // 目标用户不在线，将消息保存到文件中，待用户登录后再发送
        if(flag) {
            std::string filepath = Config::getGlobalConfig()->getConfig<std::string>("ChatServer::filepath");
            std::string filename = filepath + "tmp_" + addedname;
            std::string info = "A?" + username;
            append2File(filename, info);
        }
        return "SUCCESS";
    }

    void ChatServer::append2File(std::string& filename, std::string& str) {
        std::ofstream ofs;
        ofs.open(filename, std::ios::app);
        if(!ofs.is_open()) {
            ERROR_LOG("ChatServer::append2File() error, file open error")
            return;
        }
        ScopedLock<Spinlock> lock(m_lock);
        ofs << str << std::endl;
        ofs.close();
    }

    void ChatServer::readFile(std::string& filename, std::stringstream& ss) {
        ScopedLock<Spinlock> lock(m_lock);  
        std::ifstream ifs;
        ifs.open(filename);
        if(!ifs.is_open()) {
            FMT_INFO_LOG("failed to open file, filename: %s", filename.c_str())
            ifs.close();
            return;
        }
        std::string tmp;
        while(ifs >> tmp) {
            ss << tmp << "|";
        }
    }

    std::string ChatServer::sync(std::string& username) {
        // 同步数据
        std::string filepath = Config::getGlobalConfig()->getConfig<std::string>("ChatServer::filepath");
        std::string friendsfile = filepath + "friends_" + username;
        std::string chatfile = filepath + "chat_" + username;
        std::string opfile = filepath + "tmp_" + username;
        std::stringstream ss;
        // 依次读取好友信息、聊天信息和请求信息
        readFile(friendsfile, ss);
        ss << "/";
        readFile(chatfile, ss);
        ss << "/";
        readFile(opfile, ss);
        return ss.str();
    }

    std::string Agree(std::string& data) {
        int split_idx = data.find_first_of('/');
        std::string username = data.substr(0, split_idx);
        std::string friendname = data.substr(split_idx + 1);
        ChatServer* s = ChatServer::getGlobalChatServer();
        // 交给服务层处理
        return s->agree(username, friendname);
    }

    std::string ChatServer::agree(std::string& username, std::string& friendname) {
        // 检查是否存在id
        auto it = m_userInfos.find(friendname);
        if(it == m_userInfos.end()) {
            return "ERROR[do not have such user]";
        }
        // 目标用户存在，服务端做两件事情
        // 1. 文件中追加好友信息
        std::string filepath = Config::getGlobalConfig()->getConfig<std::string>("ChatServer::filepath");
        std::string filename1 = filepath + "friends_" + username;
        std::string filename2 = filepath + "friends_" + friendname;
        append2File(filename1, friendname);
        append2File(filename2, username);
        // 2. 向目标用户发送同意添加好友信息
        if(isOnlie(friendname)) {
            // 用户在线，发送消息
            RPCServer* rpcserver = RPCServer::getGlobalRPCServer();
            auto it = m_sessions.find(friendname);
            Response res(1, "P?" + username, "OK");
            rpcserver->sendResponse(it->second, res);
        }
        else {
            // 用户不在线，将消息保存到文件中
            std::string tmp_filename = filepath + "tmp_" + friendname;
            std::string info = "P?" + username;
            append2File(tmp_filename, info);
        }

        return "SUCCESS";
    }

    bool ChatServer::isOnlie(std::string& username) {
        auto it = m_sessions.find(username);
        if(it == m_sessions.end()) {
            return false;
        }
        RPCServer* rpcserver = RPCServer::getGlobalRPCServer();
        SocketStream::ptr session = rpcserver->getSession(it->second);
        if(session == nullptr) {
            ScopedLock<Spinlock> lock(m_lock);
            m_sessions.erase(it);
            return false;
        }
        return true;
    }

    std::string Refuse(std::string& data) {
        int split_idx = data.find_first_of('/');
        std::string username = data.substr(0, split_idx);
        std::string friendname = data.substr(split_idx + 1);
        ChatServer* s = ChatServer::getGlobalChatServer();
        // 交给服务层处理
        return s->refuse(username, friendname);
    }

    std::string ChatServer::refuse(std::string& username, std::string& friendname) {
        // 检查是否存在id
        auto it = m_userInfos.find(friendname);
        if(it == m_userInfos.end()) {
            return "ERROR[do not have such user]";
        }
        // 向目标用户发送拒绝添加好友信息
        if(isOnlie(friendname)) {
            // 用户在线，发送消息
            RPCServer* rpcserver = RPCServer::getGlobalRPCServer();
            auto it = m_sessions.find(friendname);
            Response res(1, "R?" + username, "OK");
            rpcserver->sendResponse(it->second, res);
        }
        else {
            // 用户不在线，将消息保存到文件中
            std::string filepath = Config::getGlobalConfig()->getConfig<std::string>("ChatServer::filepath");
            std::string tmp_filename = filepath + "tmp_" + friendname;
            std::string info = "R?" + username;
            append2File(tmp_filename, info);
        }

        return "SUCCESS";
    }

    // 请求数据格式: sender/receiver/message
    // 返回消息格式: I?好友用户名?message
    // message格式: [time]?message
    std::string sendMessage(std::string& data) {
        // 解析数据
        int split_idx = data.find_first_of('/'),
                split_idx2 = data.find_last_of('/');
        std::string sender = data.substr(0, split_idx);
        std::string receiver = data.substr(split_idx + 1, split_idx2 - split_idx - 1);
        std::string message = data.substr(split_idx2 + 1);
        
        return ChatServer::getGlobalChatServer()->sendMessage(sender, receiver, message);
    }

    std::string ChatServer::sendMessage(std::string& sender, std::string& receiver, std::string& message) {
        // 检查是否存在id
        auto it = m_userInfos.find(receiver);
        if(it == m_userInfos.end()) {
            return "ERROR[do not have such user]";
        }
        // 目标用户存在，根据用户是否在线做不同操作
        if(isOnlie(receiver)) {
            // 用户在线，发送消息
            RPCServer* rpcserver = RPCServer::getGlobalRPCServer();
            auto it = m_sessions.find(receiver);
            std::stringstream ss;
            ss << "I?" << sender << "?[" << getDateTime() << "]" << message;
            Response res(1, ss.str(), "OK");
            rpcserver->sendResponse(it->second, res);
        }
        else {
            // 用户不在线，将消息保存到文件中
            std::string filepath = Config::getGlobalConfig()->getConfig<std::string>("ChatServer::filepath");
            std::string tmp_filename = filepath + "chat_" + receiver;
            std::stringstream ss;
            ss << sender << "?[" << getDateTime() << "]" << message;
            std::string info = ss.str();
            append2File(tmp_filename, info);
        }

        return "SUCCESS";
    }

}