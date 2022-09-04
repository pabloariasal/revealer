#pragma once

#include "collector.h"
#include "tarball_item.h"

class PathCollector : public Collector {
public:
  explicit PathCollector(const std::filesystem::path &path_to_include)
      : path_to_include_{path_to_include} {}

  std::vector<TarballItem> collect() const override final;

private:
  std::filesystem::path path_to_include_;
};
