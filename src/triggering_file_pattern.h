#pragma once

#include <regex>

// The pattern file names must match in order to trigger a tarball creation
// event
inline const auto FILENAME_TRIGGER_PATTERN =
    std::regex("core[[:alnum:]\\.]+lz4");
