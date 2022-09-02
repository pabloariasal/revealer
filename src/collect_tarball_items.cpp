#include "collect_tarball_items.h"

std::vector<TarballItem>
collectTarballItems(const std::vector<std::unique_ptr<Collector>> &collectors) {
  (void)collectors;
  std::string s = "Hello world :)";
  auto disk = TarballItem{std::filesystem::path("disk_usage.txt"),
                          std::vector<char>(s.cbegin(), s.cend())};
  auto file1_path = "/home/pablo/tar_me/file1.txt";
  auto file1 = TarballItem{file1_path, file1_path};

  auto file2_path = "/home/pablo/tar_me/subdir/file2.txt";
  auto file2 = TarballItem{file2_path, file2_path};

  return {disk, file1, file2};
}
