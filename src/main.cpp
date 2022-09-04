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

int main(int argc, char *argv[]) {
  const auto [directory_to_observe, output_directory, paths_to_collect] =
      parseArguments(argc, argv);

  if (!std::filesystem::exists(directory_to_observe)) {
    std::cout << "[ERROR] Directory " << directory_to_observe
              << " does not exist." << std::endl;
    return EXIT_FAILURE;
  }

  if (!std::filesystem::exists(output_directory)) {
    std::cout << "[INFO] output directory" << output_directory
              << "does not exist. Creating. Creating" << std::endl;
    std::filesystem::create_directories(output_directory);
  }

  const auto collectors = getCollectors(paths_to_collect);

  auto on_new_file_created_callback = [&output_directory,
                                       &collectors](const auto &new_file) {
    if (matchesPattern(new_file.filename(), FILENAME_TRIGGER_PATTERN)) {
      auto tarball_filename =
          fmt::format("{}.tar", boost::filesystem::unique_path().string());
      auto tarball_items = collectTarballItems(collectors);
      try {
        createTarball(output_directory / tarball_filename, tarball_items);
        std::cout << "Tarball created: " << tarball_filename << std::endl;
      } catch (const std::exception &e) {
        std::cout << "[ERROR] Could not create tarball in " << tarball_filename
                  << ": " << e.what() << std::endl;
      }
    }
  };

  FileCreationListener listener{directory_to_observe,
                                on_new_file_created_callback};
  listener.listen();

  return EXIT_SUCCESS;
}
