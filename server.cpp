#include "server.hpp"

#include <cassert>
#include <iostream>

#include <arpa/inet.h>

Server::Server() : socket_(8080, 1, this), irc_controller_(new irc::IRCController(this)) {}

Server::~Server() = default;

void Server::Run() {
  while (1) {
    socket_.Poll();
  }
}

void Server::ProcessClientEvents(network::ServerPollSocket::client_id net_client,
                                 short flags) {
  if (flags & POLLIN) {
    std::string str;
    std::streambuf *buffer = socket_.GetClientBuffer(net_client);
    {
      std::istream istream(buffer);
      while (str.size() < 2 || str[str.size() - 2] != '\r') {
        std::string tmpstr;
        std::getline(istream, tmpstr, '\n');
        if (tmpstr.empty()) {
          str += "<EOF>  ";
          break;
        }
        str += tmpstr;
        str += '\n';
      }
    }
    str.resize(str.size() - 2);
    std::cout << str << std::endl;
    irc::client_id irc_client = sockets_to_users_.at(net_client);
    irc_controller_->ProcessCommand(irc_client, str);
  }
}

void Server::ClientConnected(network::ServerPollSocket::client_id net_client,
                             const sockaddr_in &info) {
  std::cout << "Client connected: " << inet_ntoa(info.sin_addr) << std::endl;
  irc::client_id irc_client = irc_controller_->ClientConnected();
  users_to_sockets_.insert({irc_client, net_client});
  sockets_to_users_.insert({net_client, irc_client});
}

void Server::ClientDisconnected(network::ServerPollSocket::client_id net_client) {
  irc::client_id irc_client = sockets_to_users_.at(net_client);
  irc_controller_->ClientDisconnected(irc_client);
  users_to_sockets_.erase(irc_client);
  sockets_to_users_.erase(net_client);
}

void Server::DropClient(irc::client_id irc_client) {
  std::cout << "IRC asked to disconnect" << irc_client << std::endl;
  network::ServerPollSocket::client_id net_client = users_to_sockets_.at(irc_client);
  socket_.DropClient(net_client);
  users_to_sockets_.erase(irc_client);
  sockets_to_users_.erase(net_client);
}

void Server::SendMessage(irc::client_id irc_client, const std::string& message) {
  network::ServerPollSocket::client_id net_client = users_to_sockets_.at(irc_client);
  std::streambuf* buffer = socket_.GetClientBuffer(net_client);
  std::ostream output(buffer);
  output << message << "\r\n";
}
