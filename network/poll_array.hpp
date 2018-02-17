#pragma once

#include <stack>
#include <vector>

#include <poll.h>

namespace network {
class PollArray {
 public:
  PollArray();

  class Iterator {
   public:
    friend Iterator& operator++(Iterator& it);
    friend struct pollfd& operator*(Iterator& it);
    friend bool operator!=(const Iterator& lhs, const Iterator& rhs);
   private:
    Iterator(size_t idx, std::vector<struct pollfd> &arr);
    // Moves to the closest element with non-empty revents
    // and idx not less than the current one.
    void GotoNextEmpty();
    size_t idx_;
    std::vector<struct pollfd>& arr_;
    friend class PollArray;
  };

  Iterator begin();
  Iterator end();

  int Poll(int timeout);
  void Insert(int fd, short events);
  void Remove(int fd);
 private:
  std::vector<struct pollfd> fds_;
  std::stack<size_t> free_;
};
}
