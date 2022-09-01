#pragma once

#include <vector>
#include <filesystem>

#include "tarball_item.h"

void createTarball(const std::filesystem::path& path, const std::vector<TarballItem>& items);

// alternatively:
/* void createTallbar(const std::filesystem::path& path, const std::vector<std::unique_ptr<Collector>>& items); */
