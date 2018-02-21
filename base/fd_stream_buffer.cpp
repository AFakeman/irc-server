#include "fd_stream_buffer.hpp"

#include <cassert>
#include <cerrno>
#include <cstring>

#include <unistd.h>

#include "base/error.hpp"

namespace base {
FdStreamBuffer::FdStreamBuffer(int fd)
    : fd_(fd),
      in_buf_(new char[buf_size_]),
      out_buf_(new char[buf_size_]),
      tmp_buf_(new char[buf_size_]) {
  setp(out_buf_.get(), out_buf_.get() + buf_size_);
  setg(in_buf_.get(), in_buf_.get() + buf_size_, in_buf_.get() + buf_size_);
}

FdStreamBuffer::FdStreamBuffer(FdStreamBuffer&& src)
    : std::streambuf(std::move(src)),
      fd_(src.fd_),
      in_buf_(std::move(src.in_buf_)),
      out_buf_(std::move(src.out_buf_)) {}

FdStreamBuffer::Traits::int_type FdStreamBuffer::overflow(int_type c) {
  size_t write_size = pptr() - pbase();
  ssize_t bytes_written = write(fd_, out_buf_.get(), write_size);
  if (bytes_written < 0) {
    if (errno == EWOULDBLOCK) {
      return Traits::eof();
    } else {
      throw base::ErrnoExcept();
    }
  }
  setp(pbase(), epptr());
  if (write_size != bytes_written) {
    size_t remainder = write_size - bytes_written;
    memcpy(tmp_buf_.get(), pptr(), remainder);
    memcpy(pbase(), tmp_buf_.get(), remainder);
    pbump(static_cast<int>(remainder));
  }
  return Traits::not_eof(c);
}

FdStreamBuffer::Traits::int_type FdStreamBuffer::underflow() {
  assert(gptr() == egptr());
  ssize_t bytes_read = read(fd_, in_buf_.get(), buf_size_);
  if (bytes_read < 0) {
    if (errno == EWOULDBLOCK) {
      return Traits::eof();
    } else {
      throw base::ErrnoExcept();
    }
  }
  setg(eback(), eback(), eback() + bytes_read);
  return bytes_read > 0 ? Traits::to_int_type(*gptr()) : Traits::eof();
}

int FdStreamBuffer::sync() {
  overflow();
  return 0;
}

}  // namespace base
