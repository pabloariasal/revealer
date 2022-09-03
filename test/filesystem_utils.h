#pragma once

#include <filesystem>
#include <fstream>
#include <string>

#include <boost/filesystem.hpp>

inline void createFile(const std::filesystem::path &path,
                       const std::string &contents) {
  std::ofstream outfile(path);
  outfile << contents << std::endl;
}

inline void createFile(const std::filesystem::path &path) {
  createFile(path, "");
}

inline std::filesystem::path unique_path() {
  return boost::filesystem::unique_path().string();
}

inline std::filesystem::path createTemporaryDirectory() {
  auto tmp_dir = std::filesystem::temp_directory_path() / unique_path();
  std::filesystem::create_directories(tmp_dir);
  return tmp_dir;
}
