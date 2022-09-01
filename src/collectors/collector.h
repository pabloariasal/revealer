#pragma once

#include <vector>

#include "tarball_item.h"

// can be triggered multiple times
// TODO: rename this to something like tarballitemcollector?
class Collector {
public:
  Collector() = default;
  virtual ~Collector() = default;

  Collector(const Collector &) = delete;
  Collector &operator=(const Collector &) = delete;
  Collector(Collector &&) = delete;
  Collector &operator=(Collector &&) = delete;

  virtual std::vector<TarballItem> collect() const = 0;

  // optionally:
  // virtual void collect(std::vector<TarBallItem>& items) const = 0;
};
