#define BOOST_NO_SCOPED_ENUMS // TODO DEPRECATED

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
  ("source", po::value<std::string>(), "Source file")
  ("destination", po::value<std::string>(), "Destination file")
  ("force", "Force to override");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  // help
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 0;
  }

  // no source
  if (vm.count("source") == 0) {
    std::cout << "No source!" << std::endl;
    return 1;
  }

  // multiple source
  if (vm.count("source") > 1) {
    std::cout << "Multiple source!" << std::endl;
    return 1;
  }

  // no destination
  if (vm.count("destination") == 0) {
    std::cout << "No destination!" << std::endl;
    return 1;
  }

  // multiple destination
  if (vm.count("destination") > 1) {
    std::cout << "Multiple destination!" << std::endl;
    return 1;
  }

  // getting values
  std::string source = vm["source"].as<std::string>();
  std::string destination = vm["destination"].as<std::string>();
  bool force = false;
  if (vm.count("force"))
    force = true;

	// executing
  if (force)
    fs::copy_file(source, destination);
  else
    fs::copy_file(source, destination, fs::copy_option::overwrite_if_exists);

  return 0;
}
