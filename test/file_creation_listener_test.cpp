#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>

#include <boost/filesystem.hpp>
#include <gtest/gtest.h>

#include "file_creation_listener.h"
#include "filesystem_utils.h"

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
  // its not very nice to detach threads but since the process terminates
  // right away it doesn't really matter
  listener_thread.detach();
}
