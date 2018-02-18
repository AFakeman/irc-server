#include "server_poll_socket.hpp"

#include <system_error>
#include <cstring>

#include <arpa/inet.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include "base/error.hpp"

namespace network {

ServerPollSocket::ServerPollSocket(int port, int backlog,
                                   ServerPollSocketDelegate* delegate)
  : delegate_(delegate) {
  sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd_ < 0) {
    throw base::ErrnoExcept();
  }
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port);
  if (bind(sockfd_, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
    throw base::ErrnoExcept();
  }
  if (listen(sockfd_, backlog) < 0) {
    throw base::ErrnoExcept();
  }
  poll_array_.Insert(sockfd_, POLLIN);
}

void ServerPollSocket::Poll() {
  poll_array_.Poll(-1);
  bool should_accept = false;
  for (const auto &fd : poll_array_) {
    if (fd.fd == sockfd_) {
      should_accept = true;
    } else {
      if (fd.revents & POLLIN) {
        delegate_->ProcessClientEvents(fd);
      }
      if (fd.revents & POLLHUP) {
        delegate_->ClientDisconnected(fd.fd);
        close(fd.fd);
        poll_array_.Remove(fd.fd); 
      }
    }
  }
  if (should_accept) {
    AcceptConnection();
  }
}

void ServerPollSocket::AcceptConnection() {
  struct sockaddr_in client_info;
  socklen_t addr_size = sizeof(client_info);
  int client_fd = accept(sockfd_, (struct sockaddr*) &client_info, &addr_size);
  if (client_fd < 0) {
    throw base::ErrnoExcept();
  }
  clients_.insert(client_fd);
  poll_array_.Insert(client_fd, POLLIN);
  delegate_->ClientConnected(client_fd, client_info);
}

ServerPollSocket::~ServerPollSocket() {
  for (int client : clients_) {
    close(client);
  }
  close(sockfd_);
}

}
