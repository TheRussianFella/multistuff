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


// Misc functions

struct Config {
  int n_threads = 1;
  bool recursive = true;
  std::string query;
  std::string start_path;

  Config(int n_threads, bool recursive,  std::string query, std::string start_path) :
    n_threads(n_threads), recursive(recursive), query(query), start_path(start_path) {};
};

Config read_command_line(int argc, char* argv[]) {

  Config conf(1, true, std::string(""), std::string("."));

  bool query_set = false;

  for (int i = 1; i < argc; ++i) {

    if (argv[i][0] == '-') {
      if (argv[i][1] == 't')
        conf.n_threads = argv[i][2] - '0';
      else if (argv[i][1] == 'n')
        conf.recursive = false;
    } else {

      if ( !query_set ) {
        conf.query = std::string( argv[i] );
        query_set = true;
      }
      else
        conf.start_path = std::string( argv[i] );
    }

  }

  assert(query_set);

  return conf;
};


// Main method

int main(int argc, char* argv[]) {

  /////// Parsing parameters

  Config conf = read_command_line(argc, argv);

  /////// Initialization

  const int MAX_WAIT_TIME = 1000;

  ParallelQueue< std::string > input_queue(MAX_WAIT_TIME);
  ParallelQueue< std::pair<std::string, int> > output_queue(MAX_WAIT_TIME);

  FileMatcher f( conf.query );

  std::promise<void> death_signal;
  std::shared_future<void>  death_future(death_signal.get_future());

  /////// Starting matchers

  std::vector <std::thread> matchers;
  for (int i = 0; i < conf.n_threads; ++i) {
    std::thread thr(f, std::ref(input_queue), std::ref(output_queue), death_future);
    matchers.push_back(std::move(thr));
  }

  /////// Starting a crawler

  FolderCrawler crl;
  std::thread crawler_thread(crl, std::ref(input_queue), conf.start_path, conf.recursive);
  crawler_thread.join();

  /////// Wait for input queue to get empty and kill matchers

  while ( input_queue.get_size() ){
    //std::cout << "Files to be processed " << input_queue.get_size() << "\n";
    std::this_thread::sleep_for( std::chrono::milliseconds(10) );
  }

  death_signal.set_value();

  for( size_t i = 0; i < matchers.size(); ++i ) {
    matchers[i].join();
  }

  /////// Printing results

  auto printer = [] (std::pair<std::string, int> pr) { return pr.first + " " + std::to_string( pr.second ); };

  std::cout << "RESULTS: \n";
  while ( output_queue.get_size() ) {
    std::cout << printer( output_queue.pop() ) << "\n";
  }

  return 0;
}
