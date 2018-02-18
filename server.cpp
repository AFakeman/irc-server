#include "server.hpp"

#include <iostream>

#include <arpa/inet.h>
#include <unistd.h>

#include "base/error.hpp"

Server::Server()
  : socket_(8080, 1, this) ,
    buffer_(new unsigned char[kBufferSize_ + 1]) {
    buffer_[kBufferSize_] = '\0';
}

Server::~Server() = default;

void Server::Run() {
  while (1) {
    socket_.Poll();
  }
}

void Server::ProcessClientEvents(const struct pollfd &fd) {
  if (fd.revents & POLLIN) {
    ssize_t read_bytes = read(fd.fd, buffer_.get(), kBufferSize_);
    if (read_bytes < 0) {
      throw base::ErrnoExcept();
    }
    buffer_[read_bytes] = '\0';
    std::cout << buffer_.get() << std::endl;
  }
}

void Server::ClientConnected(int fd, const sockaddr_in& info) {
  std::cout << "Client connected: " << inet_ntoa(info.sin_addr) << std::endl;
}

void Server::ClientDisconnected(int fd) {
  std::cout << "Client disconnected: " << fd << std::endl;
}
