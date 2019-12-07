class FileMatcher {

public:

  void operator()(ParallelQueue& shared_queue, std::string name, std::shared_future<void> death_signal) {

      while ( death_signal.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout ){
        try {

          std::string file_path(shared_queue.pop());
          std::cout << name << " got " << file_path << "\n";

        } catch (const std::runtime_error& e){

          if (e.what() != std::string("timeout"))
            throw e;

        }
      }

      std::cout << "Thread terminated...\n";
    }

};
