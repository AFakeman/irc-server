#include "server.hpp"

#include <cassert>
#include <iostream>

#include <arpa/inet.h>

Server::Server() : socket_(8080, 1, this) {}

Server::~Server() = default;

void Server::Run() {
  while (1) {
    socket_.Poll();
  }
}

void Server::ProcessClientEvents(network::ServerPollSocket::client_id client,
                                 short flags) {
  if (flags & POLLIN) {
    std::string str;
    std::streambuf *buffer = socket_.GetClientBuffer(client);
    {
      std::istream istream(buffer);
      while (str.size() < 2 || str[str.size() - 2] != '\r') {
        std::string tmpstr;
        std::getline(istream, tmpstr, '\n');
        if (tmpstr.empty()) {
          str += "<EOF>";
          break;
        }
        str += tmpstr;
        str += '\n';
      }
    }
    std::cout << str << std::endl;
    {
      std::ostream ostream(buffer);
      ostream << str << std::endl;
    }
  }
}

void Server::ClientConnected(network::ServerPollSocket::client_id client,
                             const sockaddr_in &info) {
  std::cout << "Client connected: " << inet_ntoa(info.sin_addr) << std::endl;
}

void Server::ClientDisconnected(network::ServerPollSocket::client_id client) {
  std::cout << "Client disconnected: " << client << std::endl;
}
