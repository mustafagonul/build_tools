// TODO DEPRECATED remove later
#include <boost/version.hpp>
#if BOOST_VERSION >  105100
  #define BOOST_NO_CXX11_SCOPED_ENUMS
#else
  #define BOOST_NO_SCOPED_ENUMS
#endif

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
  ("ini", po::value<std::string>(), "Ini File")
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
  std::string target = vm["target"].as<std::string>();
  std::ifstream iniFile(filename);

  if (iniFile.fail()) {
    std::cout << "Cannot open ini file!" << std::endl;
    return 1;
  }


  po::options_description iniDesc("Allowed options");
  iniDesc.add_options()
  ("header.file", po::value<std::vector<std::string>>(), "Header files")
  ("library.file", po::value<std::vector<std::string>>(), "Library files");

  po::variables_map iniVm;
  po::store(po::parse_config_file(iniFile, iniDesc), iniVm);
  po::notify(iniVm);

  if (iniVm.count("help")) {
    std::cout << desc << std::endl;
  }

  // header files
  std::vector<std::string> headerFiles;
  if (iniVm.count("header.file"))
    headerFiles = iniVm["header.file"].as<std::vector<std::string>>();

  // library files
  std::vector<std::string> libraryFiles;
  if (iniVm.count("library.file"))
    libraryFiles = iniVm["library.file"].as<std::vector<std::string>>();

  // removing current sdk
  fs::remove_all(target);

  // copying header files
  for (auto file : headerFiles) {
    fs::path source = fs::current_path();
    source /= file;

    if (fs::exists(source) == false) {
      std::cout << "Cannot find file: " << source.string() << std::endl;
      continue;
    }

    if (fs::is_regular_file(source) == false) {
      std::cout << "Not regular file: " << source.string() << std::endl;
      continue;
    }

    fs::path destination = target;
    destination /=  "include";
    destination /=  file;
    fs::create_directories(destination.parent_path());

    fs::copy_file(source, destination, fs::copy_option::overwrite_if_exists);
  }

  // copying library files
  for (auto file : libraryFiles) {
    fs::path source = fs::current_path();
    source /= file;

    if (fs::exists(source) == false) {
      std::cout << "Cannot find file: " << source.string() << std::endl;
      continue;
    }

    if (fs::is_regular_file(source) == false) {
      std::cout << "Not regular file: " << source.string() << std::endl;
      continue;
    }

    fs::path destination = target;
    destination /=  "lib";
    destination /=  file;
    fs::create_directories(destination.parent_path());

    fs::copy_file(source, destination, fs::copy_option::overwrite_if_exists);
  }

  return 0;
}
