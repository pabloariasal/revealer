#pragma once

#include <regex>
#include <string>

inline bool matchesPattern(const std::string &input,
                           const std::regex &pattern) {
  return std::regex_match(input, pattern);
}
