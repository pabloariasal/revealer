#pragma once

#include <memory>
#include <vector>

#include "tarball_item.h"
#include "collectors/collector.h"

[[nodiscard]]std::vector<TarballItem>
collectTarballItems(const std::vector<std::unique_ptr<Collector>> &collectors);
