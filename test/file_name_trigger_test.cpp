#include <string>

#include <gtest/gtest.h>

#include "pattern_matcher.h"
#include "triggering_file_pattern.h"

struct FileNameTriggerTestData {
  std::string filename;
  bool shouldTrigger;
};

std::ostream &operator<<(std::ostream &os,
                         const FileNameTriggerTestData &data) {
  os << "filename: " << data.filename
     << " should trigger: " << data.shouldTrigger;
  return os;
}

class FileNameTriggerTest
    : public ::testing::TestWithParam<FileNameTriggerTestData> {};

TEST_P(FileNameTriggerTest, TestCaseName) {
  ASSERT_EQ(matchesPattern(GetParam().filename, FILENAME_TRIGGER_PATTERN),
            GetParam().shouldTrigger);
}

INSTANTIATE_TEST_SUITE_P(
    InstantiationName, FileNameTriggerTest,
    ::testing::Values(
        FileNameTriggerTestData{
            "core.ServiceName.3057.57dd721409bc4ab4b38a3c33a36a608a."
            "3717.1647975805000000.lz4",
            true},
        FileNameTriggerTestData{"foocore.fff.lz4", false},
        FileNameTriggerTestData{"core.lz4", true},
        FileNameTriggerTestData{"core.fff.lz4", true},
        FileNameTriggerTestData{"core.fff.lz5", false},
        FileNameTriggerTestData{"", false}));
