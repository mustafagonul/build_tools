#include <iostream>
#include <boost/filesystem.hpp>


namespace fs = boost::filesystem;


int main()
{
  fs::path currentPath = fs::current_path();
  if (fs::is_directory(currentPath) == false)
    return 1;

  std::string last = currentPath.filename().string();
  if (last.empty() == true)
    return 2;

  std::cout << last << std::endl;

  return 0;
}
