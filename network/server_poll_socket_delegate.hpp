#pragma once

#include <poll.h>

class ServerPollSocketDelegate {
 public:
  virtual void ProcessClientEvents(const struct pollfd &fd) = 0;
  virtual void ClientConnected(int fd) = 0;
  virtual void ClientDisconnected(int fd) = 0;
};
