#include "error.hpp"

#include <cerrno>

namespace base {
  std::system_error ErrnoExcept() {
    return {errno, std::system_category()}; 
  }
}
