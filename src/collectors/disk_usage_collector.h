#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "collector.h"

class DiskUsageCollector : public Collector {
public:
  DiskUsageCollector(const std::filesystem::path &path_in_tarball,
                     const std::string &mount_point)
      : path_in_tarball_{path_in_tarball}, mount_point_{mount_point} {}

  // this must be lazy as things can change, if fiel does not exist warning
  std::vector<TarballItem> collect() const override final;

private:
  std::filesystem::path path_in_tarball_;
  std::string mount_point_;
};
