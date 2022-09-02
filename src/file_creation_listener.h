#pragma once

#include <iostream>
#include <filesystem>
#include <sys/inotify.h>
#include <unistd.h>

// implementation follows:
// https://www.thegeekstuff.com/2010/04/inotify-c-program-example/

template <typename CallBackT> class FileCreationListener {
public:
  FileCreationListener(const std::filesystem::path &directory_to_monitor,
                       CallBackT callback)
      : callback_{callback}, directory_to_monitor_{directory_to_monitor} {
    notify_descriptor_ = inotify_init();
    if (notify_descriptor_ < 0) {
      throw std::runtime_error("inotify_init failed");
    }
    // enter directory into the watch list
    watch_list_ = inotify_add_watch(notify_descriptor_, directory_to_monitor.c_str(), IN_CREATE);
  }

  FileCreationListener(const FileCreationListener &) = delete;
  FileCreationListener &operator=(const FileCreationListener &) = delete;
  FileCreationListener(FileCreationListener &&) = delete;
  FileCreationListener &operator=(FileCreationListener &&) = delete;

  ~FileCreationListener() noexcept {
    inotify_rm_watch(notify_descriptor_, watch_list_);
    close(notify_descriptor_);
  }

  void listenOnce() const {
    std::cout << "Starting listening to files crated in: " << directory_to_monitor_ << std::endl;
    char event_buffer[EVENT_BUF_LEN];
    auto queue_length = read(notify_descriptor_, event_buffer, EVENT_BUF_LEN);
    if (queue_length < 0) {
      throw std::runtime_error("error while reading inotify error");
    }

    auto current_event_offset{0};
    while (current_event_offset < queue_length) {
      struct inotify_event *event =
          (struct inotify_event *)&event_buffer[current_event_offset];
      if (event->len) {
        if (event->mask & IN_CREATE && !(event->mask & IN_ISDIR)) {
          callback_(std::filesystem::path{event->name});
        }
      }
      current_event_offset += EVENT_SIZE + event->len;
    }
  }

  void listen() const {
    while (true) {
      listenOnce();
    }
  }

private:
  CallBackT callback_;
  std::filesystem::path directory_to_monitor_;
  int notify_descriptor_;
  int watch_list_;

  static constexpr auto EVENT_SIZE{sizeof(struct inotify_event)};
  static constexpr auto EVENT_BUF_LEN{1024 * EVENT_SIZE};
};
