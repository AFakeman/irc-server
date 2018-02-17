#pragma once

#include <memory>
#include <unordered_set>

#include <poll.h>

#include "poll_array.hpp"
#include "server_poll_socket_delegate.hpp"

namespace network {

class ServerPollSocket {
 public:
  ServerPollSocket(int port, int backlog, ServerPollSocketDelegate* delegate);
  ~ServerPollSocket();

  // Polls the server socket and active connections
  // for new events, invoking corresponding delegate
  // actions.
  void Poll();
 private:
  void AcceptConnection();

  ServerPollSocketDelegate* delegate_;
  std::unordered_set<int> clients_;
  int sockfd_;
  PollArray poll_array_;
};

}
