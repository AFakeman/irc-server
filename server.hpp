#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "irc/irc_controller.hpp"
#include "network/server_poll_socket.hpp"

class Server : public network::ServerPollSocketDelegate,
               public irc::IRCControllerDelegate {
 public:
  Server();
  ~Server();

  void Run();

  // ServerPollSocketDelegate:
  void ProcessClientEvents(network::ServerPollSocket::client_id client,
                           short flags) override;
  void ClientConnected(network::ServerPollSocket::client_id client,
                       const sockaddr_in &info) override;
  void ClientDisconnected(network::ServerPollSocket::client_id client) override;

  // IRCControllerDelegate:
  void DropClient(irc::client_id client) override;
  void SendMessage(irc::client_id client, const std::string& message) override;

 private:
  network::ServerPollSocket socket_;
  std::unique_ptr<irc::IRCController> irc_controller_;
  std::unordered_map<irc::client_id, network::ServerPollSocket::client_id> users_to_sockets_;
  std::unordered_map<network::ServerPollSocket::client_id, irc::client_id> sockets_to_users_;
};
