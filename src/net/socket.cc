#include "socket.h"
#include "../log.h"

namespace trance {

    Socket::Socket(Domain d, Type t, int protocol) 
            : m_domain(d),
              m_type(t),
              m_protocol(protocol),
              m_sock(-1),
              isConnected(false) {
    }

    Socket::~Socket() {
      close();
    }

    void Socket::init(int sock) {
      m_sock = sock;
      int reuse = 1, enable = 1;
      if(setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        ERROR_LOG("failed to set SO_REUSEADDR")
      }
      if(setsockopt(m_sock, IPPROTO_TCP, TCP_NODELAY, &enable, sizeof(enable)) == -1) {
        ERROR_LOG("failed to set TCP_NODELAY")
      }
      sockaddr_in local, remote;
      socklen_t local_len = sizeof(local);
      int rt = getsockname(m_sock, (sockaddr*)&local, &local_len);
      if(rt == -1) {
        ERROR_LOG("failed to get local address")
      }
      socklen_t remote_len = sizeof(remote);
      rt = getpeername(m_sock, (sockaddr*)&remote, &remote_len);
      if(rt == -1) {
        ERROR_LOG("failed to get remote address")
      }
      Address::ptr p_local = std::make_shared<IPv4Address>(local);
      Address::ptr p_remote = std::make_shared<IPv4Address>(remote);
      setLocalAddress(p_local);
      setRemoteAddress(p_remote);
    }

    Socket::ptr Socket::createIPv4TCP() {
      Socket::ptr p(new Socket(IPv4, TCP));
      return p;
    }

    Socket::ptr Socket::createIPv4UDP() {
      Socket::ptr p(new Socket(IPv4, UDP));
      return p;
    }

    Socket::ptr Socket::createIPv6TCP() {
      Socket::ptr p(new Socket(IPv6, TCP));
      return p;
    }

    Socket::ptr Socket::createIPv6UDP() {
      Socket::ptr p(new Socket(IPv6, UDP));
      return p;
    }

    void Socket::bind(int port) {
      if(m_domain == IPv4) {
        if(m_sock == -1) {
          m_sock = socket(m_domain, m_type, m_protocol);
        }
        m_localAddress = IPv4Address::create("127.0.0.1", port);
        ::bind(m_sock, m_localAddress->getAddr(), m_localAddress->getAddrLen());
      }
      else {

      }
    }

    void Socket::listen(int MAXCONNECTION) {
      ::listen(m_sock, MAXCONNECTION);
    }

    Socket::ptr Socket::accept() {
      Socket::ptr p(new Socket(m_domain, m_type, m_protocol));
      int client = ::accept(m_sock, nullptr, nullptr);
      p->init(client);
      p->setConnect(true);
      FMT_INFO_LOG("a new client connected, info: %s", p->printInfo().c_str())
      return p;
    }

    void Socket::setOption() {

    }

    void Socket::connect(const Address::ptr server) {
      if(m_sock == -1) {
        m_sock = socket(m_domain, m_type, m_protocol);
      }
      int rt = ::connect(m_sock, server->getAddr(), server->getAddrLen());
      if(rt) {
        FMT_ERROR_LOG("failed to connect, server address: %s", server->toString().c_str())
        return;
      }
      init(m_sock);
      setConnect(true);
      FMT_INFO_LOG("server connected, info: %s", printInfo().c_str())
    }

    void Socket::close() {
      if(m_sock == -1) {
        return;
      }
      ::close(m_sock);
      m_sock = -1;
    }

    int Socket::send(const void* data, size_t length, int flags) {
      return ::send(m_sock, data, length, flags);
    }

    int Socket::send2(const void* data, size_t length, const Address::ptr peer, int flags) {
      return ::sendto(m_sock, data, length, flags, peer->getAddr(), peer->getAddrLen());
    }

    int Socket::recv(void* buffer, size_t length, int flags) {
      return ::recv(m_sock, buffer, length, flags);
    }

    int Socket::recvFrom(void* buffer, size_t length, Address::ptr peer, int flags) {
      socklen_t len = peer->getAddrLen();
      return ::recvfrom(m_sock, buffer, length, flags, peer->getAddr(), &len);
    }

    std::string Socket::printInfo() {
      if(isConnected) {
        return "local:" + m_localAddress->toString() + "|" + "remote:" + m_remoteAddress->toString();
      }
      else {
        INFO_LOG("not connected yet")
      }
    }
}