#pragma once

#include "network/server_poll_socket.hpp"

class Server : public network::ServerPollSocketDelegate {
 public:
  Server();

  ~Server();

  // ServerPollSocketDelegate:
  void ProcessClientEvents(network::ServerPollSocket::client_id client, short flags) override;
  void ClientConnected(network::ServerPollSocket::client_id client, const sockaddr_in& info) override;
  void ClientDisconnected(network::ServerPollSocket::client_id client) override;
 
  void Run();
 private:
  network::ServerPollSocket socket_;
};
