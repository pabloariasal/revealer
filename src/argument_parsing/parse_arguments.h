#pragma once

#include <filesystem>
#include <vector>

struct UserInput {
  std::filesystem::path observedDirectory;
  std::filesystem::path outputDirectory;
  std::vector<std::filesystem::path> pathsToCollect;
};

UserInput parseArguments(int argc, char *argv[]);
