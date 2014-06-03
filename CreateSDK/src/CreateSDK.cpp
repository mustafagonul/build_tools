#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>


namespace fs = boost::filesystem;
namespace po = boost::program_options;


int main(int argc, char **argv)
{
  po::options_description desc("Allowed options");
  desc.add_options()
  ("help", "Describe arguments")
  ("ini", po::value<std::string>(), "Ini File");
  ("target", po::value<std::string>(), "Target Directory");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  // help
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 0;
  }

  // no ini
  if (vm.count("ini") == 0) {
    std::cout << "No ini file!" << std::endl;
    return 1;
  }

  // no target
  if (vm.count("target") == 0) {
    std::cout << "No target directory!" << std::endl;
    return 1;
  }

  std::string filename = vm["ini"].as<std::string>();
  std::ifstream iniFile(filename);

  if (iniFile.fail()) {
    std::cout << "Cannot open ini file!" << std::endl;
    return 1;
  }


  po::options_description iniDesc("Allowed options");
  iniDesc.add_options()
  ("default.directory", po::value<std::vector<std::string>>(), "Default Plugin Directory")
  ("user.directory", po::value<std::vector<std::string>>(), "User Plugin Directories");

  std::ifstream ini_file("plugin.ini");
  po::variables_map vm;
  po::store(po::parse_config_file(ini_file, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
  }

  // default plugin directory
  std::string defaultDirectory;
  if (vm.count("default.directory"))
    defaultDirectory = vm["default.directory"].as<std::string>();;

  // user plugin directories
  std::vector<std::string> userDirectories;
  if (vm.count("user.directory"))
    userDirectories = vm["user.directory"].as<std::vector<std::string>>();



  return 0;
}
