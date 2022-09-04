#pragma once

#include <vector>

#include "tarball_item.h"

class Collector {
public:
  Collector() = default;
  virtual ~Collector() = default;

  Collector(const Collector &) = delete;
  Collector &operator=(const Collector &) = delete;
  Collector(Collector &&) = delete;
  Collector &operator=(Collector &&) = delete;

  virtual std::vector<TarballItem> collect() const = 0;
};
