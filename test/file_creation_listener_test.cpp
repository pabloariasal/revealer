#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#include "file_creation_listener.h"

namespace {

void createFile(const std::filesystem::path &path,
                const std::string &contents) {
  std::ofstream outfile(path);
  outfile << contents << std::endl;
}

void createFile(const std::filesystem::path &path) {
  createFile(path, "random content");
}

std::filesystem::path unique_path() {
  return boost::filesystem::unique_path().string();
}

std::filesystem::path createTemporaryDirectory() {
  auto tmp_dir = std::filesystem::temp_directory_path() / unique_path();
  std::filesystem::create_directories(tmp_dir);
  return tmp_dir;
}

} // namespace

TEST(FileCreationListenerTest, fileCreationListenerTriggersOnFileCreation) {
  auto tmp_dir = createTemporaryDirectory();
  auto recorded_file = std::filesystem::path{};
  auto callback = [&recorded_file](const auto &file) { recorded_file = file; };
  FileCreationListener listener(tmp_dir, callback);
  auto listener_thread = std::thread([&listener]() { listener.listen(); });
  for (auto file : {"file1.txt", "file2.txt"}) {
    auto file_path = tmp_dir / file;
    createFile(file_path);
    // wait some milliseconds to let the listener react to the created file
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    ASSERT_EQ(recorded_file, file_path);
  }
  // its not very nice to detach the thread but since the process terminates right
  // away it doesn't really matter
  listener_thread.detach();
}
