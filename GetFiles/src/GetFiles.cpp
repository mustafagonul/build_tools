#include <iostream>
#include <set>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>


namespace al = boost::algorithm;
namespace fs = boost::filesystem;
namespace po = boost::program_options;



void clearExtension(std::string &extension)
{
  al::trim_left(extension);
  al::trim_left_if(extension, [](char ch) {
    return ch == '.';
  });
}

void getFiles(fs::path const &path, std::string const &extensionString, std::set<std::string> &results)
try {

  // if path not exists simply returns
  if (fs::exists(path) == false)
    return;

  // if file is found
  if (fs::is_regular_file(path)) {
    if (extensionString.empty() ||
        path.extension() == '.' + extensionString) {
      auto realPath = fs::canonical(path);
      results.insert(realPath.string());
    }
  }

  // traversing directory
  if (fs::is_directory(path)) {
    fs::directory_iterator iter(path);
    fs::directory_iterator end = fs::directory_iterator();

    for (; iter != end; ++iter)
      getFiles(*iter, extensionString, results);
  }
}
catch (fs::filesystem_error const& ex)
{
  std::cout << ex.what() << '\n';
}

int main(int argc, char **argv)
{
  po::options_description desc("Allowed options");
  desc.add_options()
  ("help", "describe arguments")
  ("extension", po::value<std::string>(), "extension");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  // help
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 1;
  }

  // extension
  std::string extension;
  if (vm.count("extension"))
   extension = vm["extension"].as<std::string>();

  // executing
  fs::path currentPath = fs::current_path();
  std::set<std::string> results;
  clearExtension(extension);
  getFiles(currentPath, extension, results);

  // writing console
  std::ostream_iterator<std::string> out_it(std::cout, " ");
  std::copy(results.begin(), results.end(), out_it);


  return 0;
}
