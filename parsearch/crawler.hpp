#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;


class FolderCrawler {

public:

  void operator()(ParallelQueue<std::string>& shared_queue, std::string base_dir) {

    for (auto& p : fs::recursive_directory_iterator(base_dir) )
      shared_queue.push( p.path() );
  }

};
