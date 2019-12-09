#include <fstream>
#include <algorithm>

// TODO: Записывать результаты в очередь

class FileMatcher {

public:

  void operator()(ParallelQueue<std::string>& shared_queue, std::string query, std::shared_future<void> death_signal) {

      while ( death_signal.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout ){
        try {

          std::string file_path(shared_queue.pop());
          std::ifstream is( file_path );
          std::string temp; int line_num = 0;

          while ( std::getline(is, temp) && ++line_num ) {
            if ( temp.find(query) != std::string::npos ) {
              std::cout << "FOUND IT\n";
              std::cout << file_path << " " << line_num << "\n";
            }
          }

        } catch (const std::runtime_error& e){

          if (e.what() != std::string("timeout"))
            throw e;

        }
      }

      std::cout << "Thread terminated...\n";
    }

};
