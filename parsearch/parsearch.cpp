#include <iostream>
#include <thread>
#include <string>
#include <future>
#include <vector>
#include <chrono>
#include <string>

#include "parallel_queue.h"
#include "matcher.hpp"
#include "crawler.hpp"

int main() {

  int MAX_WAIT_TIME = 1000;

  ParallelQueue< std::string > input_queue(MAX_WAIT_TIME);
  ParallelQueue< std::pair<std::string, int> > output_queue(MAX_WAIT_TIME);

  FileMatcher f(std::string("bin"));

  std::promise<void> death_signal;
  std::shared_future<void>  death_future(death_signal.get_future());


  std::vector <std::thread> matchers;
  for (int i = 0; i < 8; ++i) {
    std::thread thr(f, std::ref(input_queue), std::ref(output_queue), std::string("bin"), death_future);
    matchers.push_back(std::move(thr));
  }

  FolderCrawler crl;
  std::thread crawler_thread(crl, std::ref(input_queue), std::string("/lib"));
  crawler_thread.join();


  while ( input_queue.get_size() ){
    //std::cout << "Files to be processed " << input_queue.get_size() << "\n";
    std::this_thread::sleep_for( std::chrono::milliseconds(10) );
  }

  death_signal.set_value();

  for( size_t i = 0; i < matchers.size(); ++i ) {
    std::cout << "started\n";
    matchers[i].join(); // Threads are not stopping again (
  }

  std::cout << "done\n";

  auto printer = [] (std::pair<std::string, int> pr) { return pr.first + " " + std::to_string( pr.second ); };

  std::cout << "RESULTS: \n";
  while ( output_queue.get_size() ) {
    std::cout << printer( output_queue.pop() ) << "\n";
  }

  return 0;
}
