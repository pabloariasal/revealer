#pragma once

#include <filesystem>
#include <variant>
#include <vector>

using Buffer = std::vector<char>;
using File = std::filesystem::path;

using Source = std::variant<File, Buffer>;

struct TarballItem {
  std::filesystem::path pathInTarball;
  Source source;
};

inline bool operator==(const TarballItem &lhs, const TarballItem &rhs) {
  return lhs.pathInTarball == rhs.pathInTarball && lhs.source == rhs.source;
}
