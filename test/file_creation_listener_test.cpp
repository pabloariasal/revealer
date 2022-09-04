#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>

#include <boost/filesystem.hpp>
#include <gtest/gtest.h>

#include "file_creation_listener.h"
#include "filesystem_utils.h"

TEST(FileCreationListenerTest, FileCreationListenerTriggersOnFileCreation) {
  auto tmp_dir = createTemporaryDirectory();
  auto reported_file_created = std::filesystem::path{};
  auto callback = [&reported_file_created](const auto &file) {
    reported_file_created = file;
  };
  FileCreationListener listener(tmp_dir, callback);
  auto listener_thread = std::thread([&listener]() { listener.listen(); });
  for (auto file : {"file1.txt", "file2.txt"}) {
    auto file_path = tmp_dir / file;
    createFile(file_path);
    // wait some milliseconds to let the listener react to the created file
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    ASSERT_EQ(reported_file_created, file_path);
  }
  // its not very nice to detach threads but since the process terminates
  // right after it doesn't really matter
  listener_thread.detach();
}
