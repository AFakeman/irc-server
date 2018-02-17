#include "poll_array.hpp"

namespace network {

PollArray::PollArray() = default;

int PollArray::Poll(int timeout) {
  return poll(fds_.data(), fds_.size(), timeout); 
}

void PollArray::Insert(int fd, short events) {
  if (free_.empty()) {
    fds_.emplace_back(pollfd{.fd=fd, .events=events});
  } else {
    size_t idx = free_.top();
    free_.pop();
    fds_[idx].fd = fd;
    fds_[idx].events = events;
  }
}

void PollArray::Remove(int fd) {
  for (size_t idx = 0; idx < fds_.size(); ++idx) {
    if (fds_[idx].fd == fd) {
      free_.push(idx);
      fds_[idx].fd = -1;
      fds_[idx].events = 0;
    }
  }
}

PollArray::Iterator::Iterator(size_t idx, std::vector<struct pollfd>& arr)
  : idx_(idx), arr_(arr) {
  GotoNextEmpty();
}

void PollArray::Iterator::GotoNextEmpty() {
  for (; idx_ < arr_.size() && arr_[idx_].revents == 0; ++idx_);
}

struct pollfd& operator*(PollArray::Iterator &it) {
  return it.arr_[it.idx_];
}

PollArray::Iterator& operator++(PollArray::Iterator &it) {
  ++it.idx_;
  it.GotoNextEmpty();
  return it;
}

bool operator!=(const PollArray::Iterator& lhs, const PollArray::Iterator& rhs) {
  return (lhs.idx_ != rhs.idx_) || (&lhs.arr_ != &rhs.arr_);
}

PollArray::Iterator PollArray::begin() {
  return PollArray::Iterator(0, fds_);
}

PollArray::Iterator PollArray::end() {
  return PollArray::Iterator(fds_.size(), fds_);
}
}
