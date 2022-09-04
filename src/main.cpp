#include <fmt/core.h>

#include <algorithm>
#include <boost/filesystem.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

#include "argument_parsing/parse_arguments.h"
#include "collect_tarball_items.h"
#include "collectors/disk_usage_collector.h"
#include "collectors/path_collector.h"
#include "create_tarball.h"
#include "file_creation_listener.h"
#include "pattern_matcher.h"
#include "triggering_file_pattern.h"

namespace {

void addPathCollectors(
    const std::vector<std::filesystem::path> &paths_to_include_in_tar,
    std::vector<std::unique_ptr<Collector>> &collectors) {
  std::transform(
      paths_to_include_in_tar.cbegin(), paths_to_include_in_tar.cend(),
      std::back_inserter(collectors),
      [](const auto &path) { return std::make_unique<PathCollector>(path); });
}

void addDiskUsageCollector(
    std::vector<std::unique_ptr<Collector>> &collectors) {
  collectors.push_back(
      std::make_unique<DiskUsageCollector>("disk_usage.txt", "/"));
}

std::vector<std::unique_ptr<Collector>> getCollectors(
    const std::vector<std::filesystem::path> &paths_to_include_in_tar) {
  std::vector<std::unique_ptr<Collector>> collectors;
  collectors.reserve(paths_to_include_in_tar.size() + 1);
  addPathCollectors(paths_to_include_in_tar, collectors);
  addDiskUsageCollector(collectors);
  return collectors;
}

void createOutputDirIfNotExists(const std::filesystem::path &output_directory) {
  if (!std::filesystem::exists(output_directory)) {
    std::cout << "[INFO] output directory" << output_directory
              << "does not exist. Creating. Creating" << std::endl;
    std::filesystem::create_directories(output_directory);
  }
}
} // namespace

int main(int argc, char *argv[]) {
  const auto [directory_to_observe, output_directory, paths_to_collect] =
      parseArguments(argc, argv);
  if (!std::filesystem::exists(directory_to_observe)) {
    std::cout << "[ERROR] Directory does not exist: " << directory_to_observe
              << std::endl;
    return EXIT_FAILURE;
  }
  createOutputDirIfNotExists(output_directory);

  const auto collectors = getCollectors(paths_to_collect);

  auto on_new_file_created_callback = [&output_directory,
                                       &collectors](const auto &new_file) {
    if (matchesPattern(new_file.filename(), FILENAME_TRIGGER_PATTERN)) {
      auto tarball_path =
          output_directory /
          fmt::format("{}.tar", boost::filesystem::unique_path().string());
      try {
        createTarball(tarball_path, collectTarballItems(collectors));
        std::cout << "Tarball created: " << tarball_path << std::endl;
      } catch (const std::exception &e) {
        std::cout << "[ERROR] Tarball creation failed in " << tarball_path
                  << ": " << e.what() << std::endl;
      }
    }
  };

  FileCreationListener listener{directory_to_observe,
                                on_new_file_created_callback};
  listener.listen();

  return EXIT_SUCCESS;
}
