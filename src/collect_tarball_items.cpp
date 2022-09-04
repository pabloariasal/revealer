#include "collect_tarball_items.h"

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>

std::vector<TarballItem>
collectTarballItems(const std::vector<std::unique_ptr<Collector>> &collectors) {
  // clang-format off
  return collectors |
         ranges::views::transform([](const auto &collector) { return collector->collect(); }) |
         ranges::views::join | // flatten a vec of vec into a vec
         ranges::to<std::vector>();
  // clang-format on
}
