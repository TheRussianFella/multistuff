class FolderCrawler {

public:

  void operator()(ParallelQueue& shared_queue) {

    for (int i = 0; i < 20; ++i) {
      shared_queue.push( std::string("my string ") + std::to_string(i) );
      std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
  }

};
