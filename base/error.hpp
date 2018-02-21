#include <system_error>

namespace base {
// Returns an exception corresponding to the
// current errno value.
std::system_error ErrnoExcept();
}  // namespace base
