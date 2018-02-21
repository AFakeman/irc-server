#pragma once

#include <memory>
#include <streambuf>

namespace base {
// A class for reading/writing to Unix file descriptors.
// Does not assume ownership over them, but as
// it buffers reads/writes should be the only one
// reading from/writing to it.
class FdStreamBuffer : public std::streambuf {
 public:
  FdStreamBuffer(int fd);
  FdStreamBuffer(const FdStreamBuffer& src) = delete;
  FdStreamBuffer(FdStreamBuffer&& src);

 protected:
  typedef std::char_traits<char> Traits;

  Traits::int_type overflow(int_type c = Traits::eof());
  Traits::int_type underflow();
  int sync();

 private:
  const int fd_;
  const size_t buf_size_ = 512;
  std::unique_ptr<char[]> in_buf_;
  std::unique_ptr<char[]> out_buf_;
  std::unique_ptr<char[]> tmp_buf_;
};
}  // namespace base
