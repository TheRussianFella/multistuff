#include "parallel_queue.h"

template <typename T>
inline
int ParallelQueue<T>::push(const T& value) {

  std::lock_guard<std::mutex> lck( queue_mutex );
  queue.push( std::move(value) );
  size += 1;
  not_empty.notify_one();

  return 0;
}

template <typename T>
inline
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
