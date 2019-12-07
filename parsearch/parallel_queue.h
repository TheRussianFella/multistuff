#include <queue>
#include <string>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <iostream>

#ifndef PARALLELQUEUE
#define PARALLELQUEUE

class ParallelQueue {

private:

  std::queue<std::string> queue;
  std::mutex queue_mutex;
  std::condition_variable not_empty;
  int MAX_WAIT_TIME;
  size_t size;

public:

  ParallelQueue( int MAX_WAIT_TIME = 500 ) : MAX_WAIT_TIME(MAX_WAIT_TIME), size(0) {};

  int push(const std::string& value);
  std::string pop();

};

#endif
