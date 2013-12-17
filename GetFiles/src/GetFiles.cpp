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

void getFilesWithExtension(fs::path const &path, std::string const &extension, std::vector<std::string> const &ignoreList, std::set<std::string> &results)
try {

  // if path not exists simply returns
  if (fs::exists(path) == false)
    return;

  // if file is found
  if (fs::is_regular_file(path)) {
    if (extension.empty() ||
        path.extension() == '.' + extension) {
      //auto realPath = fs::canonical(path);
      //results.insert(realPath.string());
      results.insert(path.string());
    }
  }

  // traversing directory
  if (fs::is_directory(path)) {
    for (auto const &ignore : ignoreList)
      if (ignore == path.stem().string())
        return;

    fs::directory_iterator iter(path);
    fs::directory_iterator end = fs::directory_iterator();
    std::vector<std::string> emptyIgnoreList;
    for (; iter != end; ++iter)
      getFilesWithExtension(*iter, extension, emptyIgnoreList, results);
  }
}
catch (fs::filesystem_error const& ex)
{
  std::cout << ex.what() << '\n';
}

void getFilesWithEnding(fs::path const &path, std::string const &ending, std::vector<std::string> const &ignoreList, std::set<std::string> &results)
try {

  // if path not exists simply returns
  if (fs::exists(path) == false)
    return;

  // if file is found
  if (fs::is_regular_file(path)) {
    auto pathString = path.string();
    auto pathStringLength = pathString.length();
    auto endingLength = ending.length();
    if (endingLength > pathStringLength)
     return;

    auto pathStringEnding = pathString.substr(pathStringLength - endingLength);
    if (pathStringEnding == ending) {
     //auto realPath = fs::canonical(path);
     //results.insert(realPath.string());
     results.insert(path.string());
    }
  }

  // traversing directory
  if (fs::is_directory(path)) {
    for (auto const &ignore : ignoreList)
      if (ignore == path.stem().string())
        return;

    fs::directory_iterator iter(path);
    fs::directory_iterator end = fs::directory_iterator();
    std::vector<std::string> emptyIgnoreList;
    for (; iter != end; ++iter)
      getFilesWithEnding(*iter, ending, emptyIgnoreList, results);
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
  ("help", "Describe arguments")
  ("extension", po::value<std::string>(), "Extension")
  ("ending", po::value<std::string>(), "Ending")
  ("ignore-directory,I", po::value<std::vector<std::string>>(), "Ignore Directory");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  // help
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 0;
  }

  // no parameter
  if (vm.count("extension") == 0 && vm.count("ending") == 0) {
    std::cout << "There is no parameter!" << std::endl;
    return 1;
  }

  // multiple parameter
  if (vm.count("extension") && vm.count("ending")) {
    std::cout << "Multiple parameters!" << std::endl;
    return 1;
  }

  // ignore list
  std::vector<std::string> ignoreList;
  if (vm.count("ignore-directory"))
    ignoreList = vm["ignore-directory"].as<std::vector<std::string>>();

  //
  fs::path currentPath = fs::current_path();
  std::set<std::string> results;

  // extension
  if (vm.count("extension")) {
    std::string extension = vm["extension"].as<std::string>();
    clearExtension(extension);
    getFilesWithExtension(currentPath, extension, ignoreList, results);
  }

  // ending
  if (vm.count("ending")) {
    std::string ending = vm["ending"].as<std::string>();
    getFilesWithEnding(currentPath, ending, ignoreList, results);
  }

  // writing to console
  std::ostream_iterator<std::string> out_it(std::cout, " ");
  //std::ostream_iterator<std::wstring, wchar_t> out_it(std::wcout, L" ");
  std::copy(results.begin(), results.end(), out_it);

  return 0;
}
