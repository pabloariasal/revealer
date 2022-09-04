#pragma once

#include <filesystem>
#include <vector>

#include "tarball_item.h"

void createTarball(const std::filesystem::path &path,
                   const std::vector<TarballItem> &items);
