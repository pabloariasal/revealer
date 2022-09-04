#pragma once

#include <vector>

#include "tarball_item.h"

/**
 * A Collector is an object that is responsible for adding entries to a tarball
 * The idea is that every information that must be present in the tarball by a
 * different Collector.
 *
 * For example, a PathCollector adds a specific file or directory from the file
 * system to the tarball, and a DiskUsageCollector adds an entry containing disk
 * usage information, etc.
 *
 * New collectors can be easily added by implementing the interface.
 *
 */
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
