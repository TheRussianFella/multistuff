#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;


class FolderCrawler {

public:

  void operator()(ParallelQueue<std::string>& shared_queue, std::string base_dir, bool recursive) {

    std::vector<std::string> next_directories; next_directories.push_back(base_dir);

    while ( next_directories.size() ) {

      std::string temp(next_directories.back()); next_directories.pop_back();

      for (auto& p : fs::directory_iterator(temp, fs::directory_options::skip_permission_denied) ) {

        if ( fs::is_directory(p.path()) )
          next_directories.push_back(p.path());
        else
          shared_queue.push(p.path());

      }

      if (!recursive)
        break;
    }
  }

};
