#include <iostream>
#include <thread>
#include <string>
#include <future>
#include <vector>
#include <chrono>
#include <assert.h>
#include <queue>
#include <string>
#include <mutex>
#include <chrono>
#include <condition_variable>


class ParallelQueue {

private:

  std::queue<std::string> queue;
  std::mutex queue_mutex;
  std::condition_variable not_empty;
  size_t size;
  int MAX_WAIT_TIME;

public:

  ParallelQueue( int MAX_WAIT_TIME = 500 ) : MAX_WAIT_TIME(MAX_WAIT_TIME), size(0) {};

  int push(const std::string& value);
  std::string pop();

};


int ParallelQueue::push(const std::string& value) {

  std::lock_guard<std::mutex> lck( queue_mutex );
  queue.push( std::move(value) );
  size += 1;
  not_empty.notify_one();

  return 0;
}

std::string ParallelQueue::pop() {

  // If queue is empty we just wait for someone to add items into it.
  std::unique_lock<std::mutex> lck( queue_mutex );
  not_empty.wait_for(lck, std::chrono::milliseconds(MAX_WAIT_TIME), [this]()->bool{ return size; });

  // We've reached time out and should quit
  if(!size) {
    throw std::runtime_error("timeout");
  }

  std::string result(queue.front());
  queue.pop();
  size -= 1;
  return result;

}



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

class FolderCrawler {

public:

  void operator()(ParallelQueue& shared_queue) {

    for (int i = 0; i < 4; ++i) {
      std::cout << "Making string...\n";
      shared_queue.push( std::string("my string ") + std::to_string(i) );
      std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
  }

};

int main() {

  int MAX_WAIT_TIME = 1000;
  ParallelQueue q(MAX_WAIT_TIME);
  FileMatcher f;

  std::promise<void> death_signal;
  std::shared_future<void>  death_future(death_signal.get_future());

  std::cout << "Creating threads!\n";

  std::vector <std::thread> matchers;
  for (int i = 0; i < 4; ++i) {
    std::thread thr(f, std::ref(q), std::string("Thread ")+std::to_string(i), death_future);
    matchers.push_back(std::move(thr));
  }

  std::cout << "Waiting for directories!\n";
  FolderCrawler crl;
  std::thread crawler_thread(crl, std::ref(q));
  crawler_thread.join();

  std::cout << "Terminating!\n";
  death_signal.set_value();

  for( size_t i = 0; i < matchers.size(); ++i)
    matchers[i].join();

  return 0;
}
