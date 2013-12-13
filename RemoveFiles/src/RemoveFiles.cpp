#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>


namespace fs = boost::filesystem;
namespace po = boost::program_options;


int main(int argc, char **argv)
{
  po::options_description desc("Allowed options");
  desc.add_options()
  ("help", "Describe arguments")
  ("files", po::value<std::vector<std::string>>(), "Files to remove");

  po::positional_options_description p;
  p.add("files", -1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
  po::notify(vm);

  // help
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 0;
  }

  // no file
  if (vm.count("files") == 0) {
    std::cout << "No File!" << std::endl;
    return 1;
  }

  auto files = vm["files"].as<std::vector<std::string>>();
  for (auto &file : files)
    fs::remove(file);

  return 0;
}
