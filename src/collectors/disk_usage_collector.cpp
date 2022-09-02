#include "disk_usage_collector.h"

#include <cmath>
#include <iostream>

#include <fmt/core.h>
#include <sys/statvfs.h>

// reference: https://www.ibm.com/docs/en/zos/2.1.0?topic=functions-statvfs-get-file-system-information

constexpr auto DISK_REPORT_FMT = "Disk usage report\n"
                                 "Mount Point: {}\n"
                                 "Used: {:.2f}GB\n"
                                 "Free: {:.2f}GB\n"
                                 "Total: {:.2f}GB";

std::vector<TarballItem> DiskUsageCollector::collect() const {

  struct statvfs buffer;
  int ret = statvfs(mount_point_.c_str(), &buffer);
  if (ret < 0) {
    throw std::runtime_error("error occurred when gathering disk usage");
  }
  auto total_available_bytes =
      static_cast<double>(buffer.f_blocks) * buffer.f_bsize;
  auto total_free_bytes = static_cast<double>(buffer.f_bavail) * buffer.f_bsize;
  auto total_used_bytes = total_available_bytes - total_free_bytes;

  static constexpr auto GB_IN_BYTES = 1073741824;
  auto report = fmt::format(
      DISK_REPORT_FMT, mount_point_, total_used_bytes / GB_IN_BYTES,
      total_free_bytes / GB_IN_BYTES, total_available_bytes / GB_IN_BYTES);
  return {
      {path_in_tarball_, std::vector<char>(report.cbegin(), report.cend())}};
}
