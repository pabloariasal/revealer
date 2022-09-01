#pragma once

#include <filesystem>

template <typename CallBackT> class FileCreationListener {
public:
  FileCreationListener(const std::filesystem::path &directory_to_monitor,
                       CallBackT callback)
      : callback_{callback}, directory_to_monitor_{directory_to_monitor} {}
  void run() {}

private:
  CallBackT callback_;
  std::filesystem::path directory_to_monitor_;
};
