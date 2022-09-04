#include "create_tarball.h"

#include <fstream>
#include <iostream>

#include <fmt/core.h>

#include "microtar.h"

namespace {

std::vector<char> readFile(const std::filesystem::path &file_path) {
  std::ifstream file(file_path.string(), std::ios::binary | std::ios::ate);
  // enable exceptions on bad and fail bits, this frees us from having to check
  // error codes after every i/o operation. See:
  // https://cplusplus.com/reference/ios/ios/exceptions/
  file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<char> buffer(size);
  file.read(buffer.data(), size);
  return buffer;
}

/**
 * Opens a stream to a tarball for writing, flushes and closes on destruction.
 */
class TarBallWritter {
public:
  TarBallWritter(const std::filesystem::path &path) : path_{path} {
    check(mtar_open(&tar_, path.c_str(), "w"));
  }

  TarBallWritter(const TarBallWritter &) = delete;
  TarBallWritter &operator=(const TarBallWritter &) = delete;
  TarBallWritter(TarBallWritter &&) = delete;
  TarBallWritter &operator=(TarBallWritter &&) = delete;

  void add(const std::filesystem::path &path_in_tarball,
           const std::vector<char> &buffer) {
    auto buff_size = sizeof(char) * buffer.size();
    check(mtar_write_file_header(&tar_, path_in_tarball.c_str(), buff_size));
    check(mtar_write_data(&tar_, buffer.data(), buff_size));
  }

  ~TarBallWritter() {
    mtar_finalize(&tar_);
    mtar_close(&tar_);
  }

private:
  void check(int status) const {
    if (status != MTAR_ESUCCESS) {
      throw std::runtime_error{
          fmt::format("Error when writing tarball in '{}'. Error code: {}",
                      path_.string(), status)};
    }
  }
  std::filesystem::path path_;
  mtar_t tar_;
};

// see: https://en.cppreference.com/w/cpp/utility/variant/visit
template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

} // namespace

void createTarball(const std::filesystem::path &path,
                   const std::vector<TarballItem> &items) {
  TarBallWritter writter{path};
  for (const auto &[path_in_tarball, source] : items) {
    std::visit(
        overloaded{
            [&path_in_tarball, &writter](const std::filesystem::path &path) {
              if (!std::filesystem::exists(path)) {
                throw std::runtime_error{
                    fmt::format("{} does not exist.", path.string())};
              }
              writter.add(path_in_tarball, readFile(path));
            },
            [&path_in_tarball, &writter](const std::vector<char> &buffer) {
              writter.add(path_in_tarball, buffer);
            }},
        source);
  }
}
