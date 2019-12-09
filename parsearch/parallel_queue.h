#include <queue>
#include <string>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <iostream>

#ifndef PARALLELQUEUE
#define PARALLELQUEUE

template <typename T>
class ParallelQueue {

private:

  std::queue< T > queue;
  std::mutex queue_mutex;
  std::condition_variable not_empty;
  int MAX_WAIT_TIME;
  size_t size;

public:

  ParallelQueue( int MAX_WAIT_TIME = 500 ) : MAX_WAIT_TIME(MAX_WAIT_TIME), size(0) {};

  int push(const T& value);
  T pop();

};


template <typename T>
int ParallelQueue<T>::push(const T& value) {

  std::lock_guard<std::mutex> lck( queue_mutex );
  queue.push( std::move(value) );
  size += 1;
  not_empty.notify_one();

  return 0;
}

template <typename T>
T ParallelQueue<T>::pop() {

  // If queue is empty we just wait for someone to add items into it.
  std::unique_lock<std::mutex> lck( queue_mutex );
  not_empty.wait_for(lck, std::chrono::milliseconds(MAX_WAIT_TIME), [this]()->bool{ return size; });

  // We've reached time out and should quit
  if(!size) {
    throw std::runtime_error("timeout");
  }

  T result(queue.front());
  queue.pop();
  size -= 1;
  return result;

}

#endif
