#include "parse_arguments.h"

#include <iostream>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace {

po::variables_map parseCommandLine(int argc, char *argv[]) {
  po::options_description description("revealer");
  // clang-format off
  description.add_options()
  ("help", "Display this help message")
  ("observe-directory", po::value<std::string>()->required(), "Directory to observe for file creation.")
  ("output-directory", po::value<std::string>()->required(), "Directory the tarball will be saved to.")
  ("paths-to-include", po::value<std::vector<std::string>>()->required(), "Files or directories to be included in the tarball.")
  ;
  // clang-format on
  po::positional_options_description positional;
  positional.add("paths-to-include", -1);
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv)
                .options(description)
                .positional(positional)
                .run(),
            vm);
  if (argc == 1 || vm.count("help")) {
    std::cout << description << std::endl;
    exit(0);
  }
  po::notify(vm);
  return vm;
}
} // namespace

UserInput parseArguments(int argc, char *argv[]) {
  auto vm = parseCommandLine(argc, argv);
  auto observe_directory =
      std::filesystem::path{vm["observe-directory"].as<std::string>()};
  auto output_directory =
      std::filesystem::path{vm["output-directory"].as<std::string>()};
  auto paths = vm["paths-to-include"].as<std::vector<std::string>>();
  std::vector<std::filesystem::path> paths_to_include;
  paths_to_include.reserve(paths.size());
  std::transform(paths.cbegin(), paths.cend(),
                 std::back_inserter(paths_to_include), [](const auto p) {
                   return std::filesystem::absolute(std::filesystem::path{p});
                 });
  return {std::filesystem::absolute(observe_directory),
          std::filesystem::absolute(output_directory),
          std::move(paths_to_include)};
}
