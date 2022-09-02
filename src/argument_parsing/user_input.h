#pragma once

#include <filesystem>
#include <vector>

struct UserInput {
  std::filesystem::path observedDirectory;
  std::filesystem::path outputDirectory;
  std::vector<std::filesystem::path> pathsToCollect;
};

inline std::ostream &operator<<(std::ostream &os, const UserInput &user_input) {
  os << "observed directory: " << user_input.observedDirectory << '\n'
     << "output directory: " << user_input.outputDirectory << '\n'
     << "paths to be collected:" << '\n';
  for (const auto &path : user_input.pathsToCollect) {
    os << path << '\n';
  }
  return os;
}
