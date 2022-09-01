#pragma once

#include <filesystem>

#include "collector.h"

class DiskUsageCollector : public Collector {
public:
  DiskUsageCollector(const std::filesystem::path &path_in_tarball)
      : path_in_tarball_{path_in_tarball} {}

  ~DiskUsageCollector() override = default;

  // this must be lazy as things can change, if fiel does not exist warning
  std::vector<TarballItem> collect() const override;

private:
  std::filesystem::path path_in_tarball_;
};
