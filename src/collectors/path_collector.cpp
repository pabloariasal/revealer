#include "path_collector.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <vector>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>

namespace {

std::vector<std::filesystem::path>
getAllFilesInDirectoryRecursively(const std::filesystem::path &path) {
  std::vector<std::filesystem::path> files_in_dir;
  auto directory_it = std::filesystem::recursive_directory_iterator(path);
  std::copy_if(std::filesystem::begin(directory_it),
               std::filesystem::end(directory_it),
               std::back_inserter(files_in_dir),
               [](const auto &entry) { return !entry.is_directory(); });
  return files_in_dir;
}

} // namespace

std::vector<TarballItem> PathCollector::collect() const {
  if (!std::filesystem::exists(path_to_include_)) {
    std::cout << "[WARN] " << path_to_include_
              << " does not exist. It won't be included in the tarball"
              << std::endl;
    return {};
  }

  if (std::filesystem::is_empty(path_to_include_)) {
    std::cout << "[WARN] " << path_to_include_
              << " is empty. It won't be included in the tarball" << std::endl;
    return {};
  }

  if (!std::filesystem::is_directory(path_to_include_)) {
    return {{path_to_include_, path_to_include_}};
  }

  auto files_to_include = getAllFilesInDirectoryRecursively(path_to_include_);
  // clang-format off
  return files_to_include |
         ranges::views::transform([](const auto& file_path){ return TarballItem{file_path, file_path}; }) |
         ranges::to<std::vector>();
  // clang-format on
}
