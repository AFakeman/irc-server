#pragma once

#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include <poll.h>
#include <netinet/in.h>

#include "poll_array.hpp"

namespace base {
    class FdStreamBuffer;
}

namespace network {

class ServerPollSocketDelegate;

// Class for base networking, providing control for
// accepting/dropping connections, and polling them
// for new information.
class ServerPollSocket {
 public:
  ServerPollSocket(int port, int backlog, ServerPollSocketDelegate* delegate);
  ~ServerPollSocket();

  // Type used to store ID of a client. Not guranteed to
  // be unique, may be reused for a new connection 
  // if a client with this ID was disconnected.
  typedef int client_id;

  // Polls the server socket and active connections
  // for new events, invoking corresponding delegate
  // actions.
  void Poll();

  // Get the read/write buffer associated with the client.
  std::streambuf* GetClientBuffer(client_id client); 

  // Get the set of all clients.
  const std::unordered_set<int>* GetClients() const;
 private:
  void AcceptConnection();

  ServerPollSocketDelegate* delegate_;
  int sockfd_;
  std::unordered_map<int, std::unique_ptr<base::FdStreamBuffer>> client_buffers_;
  std::unordered_set<client_id> client_ids_;
  PollArray poll_array_;
};

class ServerPollSocketDelegate {
 public:
  virtual void ProcessClientEvents(ServerPollSocket::client_id, short flags) = 0;
  virtual void ClientConnected(ServerPollSocket::client_id client, const sockaddr_in& info) = 0;
  virtual void ClientDisconnected(ServerPollSocket::client_id client) = 0;
};
}
