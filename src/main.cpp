#include <algorithm>
#include <cstdlib>
#include <memory>
#include <vector>

#include "argument_parsing/parse_arguments.h"
#include "collect_tarball_items.h"
#include "collectors/disk_usage_collector.h"
#include "collectors/path_collector.h"
#include "create_tarball.h"
#include "file_creation_listener.h"
#include "pattern_matcher.h"
#include "utils.h"
#include "valid_file_pattern.h"

void addPathCollectors(
    const std::vector<std::filesystem::path> &paths_to_include_in_tar,
    std::vector<std::unique_ptr<Collector>> &collectors) {
  std::transform(
      paths_to_include_in_tar.cbegin(), paths_to_include_in_tar.cend(),
      std::back_inserter(collectors),
      [](const auto &path) { return std::make_unique<PathCollector>(path); });
}

void addDiskUsageCollector(
    const std::filesystem::path &path_in_tar,
    std::vector<std::unique_ptr<Collector>> &collectors) {
  collectors.push_back(std::make_unique<DiskUsageCollector>(path_in_tar));
}

std::vector<std::unique_ptr<Collector>> getCollectors(
    const std::vector<std::filesystem::path> &paths_to_include_in_tar) {
  std::vector<std::unique_ptr<Collector>> collectors;
  collectors.reserve(paths_to_include_in_tar.size() + 1);
  addPathCollectors(paths_to_include_in_tar, collectors);
  addDiskUsageCollector("/disk_usage.txt", collectors);
  return collectors;
}

int main(int argc, char *argv[]) {
  const auto [directory_to_observe, output_directory, paths_to_collect] =
      parseArguments(argc, argv);

  const auto collectors = getCollectors(paths_to_collect);

  auto on_new_file_created_callback = [&output_directory,
                                       &collectors](const auto &new_file) {
    if (matchesPattern(new_file.filename(), VALID_FILE_PATTERN)) {
      auto tarball_filename = output_directory / (randomName() + ".tar");
      auto tarball_items = collectTarballItems(collectors);
      createTarball(tarball_filename, tarball_items);
    }
  };

  FileCreationListener listener{directory_to_observe,
                                on_new_file_created_callback};
  listener.listen();

  return EXIT_SUCCESS;
}
