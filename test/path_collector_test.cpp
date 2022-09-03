#include <filesystem>
#include <fstream>

#include <iostream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "collectors/path_collector.h"
#include "filesystem_utils.h"

class PathCollectorTest : public ::testing::Test {
protected:
  void SetUp() override {
    // ├── file1.txt
    // └── subdir
    //    ├── file2.txt
    //    └── subsubdir
    //       └── file3.txt -> ../file2.txt
    std::filesystem::path FILE1{"file1.txt"};
    std::filesystem::path FILE2{"subdir/file2.txt"};
    std::filesystem::path FILE3{"subdir/subsubdir/file3.txt"};

    std::filesystem::current_path(createTemporaryDirectory());
    std::filesystem::create_directories(FILE3.parent_path());
    createFile(FILE1);
    createFile(FILE2);
    std::filesystem::create_symlink("../file2.txt", FILE3);

    file1_ = std::filesystem::absolute(FILE1);
    file2_ = std::filesystem::absolute(FILE2);
    file3_ = std::filesystem::absolute(FILE3);
    std::cout << "pwd: " << std::filesystem::current_path() << std::endl;
  }
  void TearDown() override {}
  std::filesystem::path file1_;
  std::filesystem::path file2_;
  std::filesystem::path file3_;
};

TEST_F(PathCollectorTest, pathDoesNotExist) {
  PathCollector collector{"some/path/that/does/not/exist.txt"};
  ASSERT_TRUE(collector.collect().empty());
}

TEST_F(PathCollectorTest, directoryIsEmpty) {
  PathCollector collector{createTemporaryDirectory()};
  ASSERT_TRUE(collector.collect().empty());
}

TEST_F(PathCollectorTest, AddSingleFile) {
  PathCollector collector{file1_};
  ASSERT_TRUE(::testing::Matches(::testing::ElementsAre(
      TarballItem{file1_, file1_}))(collector.collect()));
}

TEST_F(PathCollectorTest, AddDirectory) {
  PathCollector collector{std::filesystem::current_path()};
  auto expected_items = std::vector<TarballItem>{
      {file1_, file1_}, {file2_, file2_}, {file3_, file3_}};
  ASSERT_TRUE(::testing::Matches(::testing::UnorderedElementsAreArray(
      expected_items))(collector.collect()));
}
