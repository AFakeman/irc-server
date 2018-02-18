#pragma once

#include <memory>

#include "network/server_poll_socket.hpp"
#include "network/server_poll_socket_delegate.hpp"

class Server : public network::ServerPollSocketDelegate {
 public:
  Server();

  ~Server();

  // ServerPollSocketDelegate:
  void ProcessClientEvents(const struct pollfd &fd);
  void ClientConnected(int fd, const sockaddr_in& info);
  void ClientDisconnected(int fd);
 
  void Run();
 private:
  static const size_t kBufferSize_ = 512;
  network::ServerPollSocket socket_;  
  std::unique_ptr<unsigned char[]> buffer_;
};
