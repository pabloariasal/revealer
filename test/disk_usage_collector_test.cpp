#include <filesystem>
#include <iostream>
#include <regex>

#include <gtest/gtest.h>

#include "collectors/disk_usage_collector.h"

TEST(DiskUsageCollector, reportsUsage) {
  const auto path_in_tarball = std::filesystem::path{"disk_usage.txt"};
  DiskUsageCollector disk_usage_collector{path_in_tarball, "/"};
  auto actual = disk_usage_collector.collect();
  ASSERT_EQ(1, actual.size());
  ASSERT_EQ(actual.front().pathInTarball, path_in_tarball);
  auto generated_report_bytes = std::get<Buffer>(actual.front().source);
  auto generated_report = std::string(generated_report_bytes.cbegin(),
                                      generated_report_bytes.cend());
  std::cout << generated_report << std::endl;
  ASSERT_TRUE(
      std::regex_search(generated_report, std::regex("Mount Point: /")));
  for (auto type : {"Used", "Free", "Total"}) {
    ASSERT_TRUE(std::regex_search(
        generated_report,
        std::regex(std::string(type) + ": (\\d)+\\.\\d{2}GB")));
  }
}
