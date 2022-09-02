#pragma once

#include "collector.h"
#include "tarball_item.h"

class PathCollector : public Collector {
public:
  PathCollector(const std::filesystem::path &path_to_include)
      : path_to_include_{path_to_include} {}
  ~PathCollector() override = default;

  // this must be lazy as things can change, if fiel does not exist warning
  std::vector<TarballItem> collect() const override;

            // what happens if a file to be collected does not exist?
private:
  std::filesystem::path path_to_include_;
};
