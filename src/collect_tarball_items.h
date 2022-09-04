#pragma once

#include <memory>
#include <vector>

#include "collectors/collector.h"
#include "tarball_item.h"

[[nodiscard]] std::vector<TarballItem>
collectTarballItems(const std::vector<std::unique_ptr<Collector>> &collectors);
