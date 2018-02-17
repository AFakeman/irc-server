#pragma once

#include <memory>

#include "poll_server_socket.hpp"
#include "server_poll_socket_delegate.hpp"

class Server : public ServerPollSocketDelegate {
 public:
  Server();

  ~Server();

  // ServerPollSocketDelegate:
  void ProcessClientEvents(const struct pollfd &fd);
  void ClientConnected(int fd);
  void ClientDisconnected(int fd);
 
  void Run();
 private:
  static const size_t kBufferSize_ = 512;
  ServerPollSocket socket_;  
  std::unique_ptr<unsigned char[]> buffer_;
};
