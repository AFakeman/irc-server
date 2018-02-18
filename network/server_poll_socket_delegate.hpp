#pragma once

#include <poll.h>
#include <netinet/in.h>

namespace network {
class ServerPollSocketDelegate {
 public:
  virtual void ProcessClientEvents(const struct pollfd &fd) = 0;
  virtual void ClientConnected(int fd, const sockaddr_in& info) = 0;
  virtual void ClientDisconnected(int fd) = 0;
};
}
