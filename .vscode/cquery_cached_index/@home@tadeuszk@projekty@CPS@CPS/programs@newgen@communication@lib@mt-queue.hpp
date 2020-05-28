#ifndef MT_QUEUE_HPP
#define MT_QUEUE_HPP

#include <mutex>
#include <queue>

/* Custom Thread safe queue */
template <typename T>
class MT_Queue
{
private:
  std::queue<T> container;
  std::mutex m;
public:
  MT_Queue(){};
  void push(T value){
    std::lock_guard<std::mutex> lock(m);
    container.push(value);
  }

  void pop(T &result){
    std::lock_guard<std::mutex> lock(m);
    result = container.front();
    container.pop();
  }

  int size(void){
    std::lock_guard<std::mutex> lock(m);
    return container.size();
  }

  bool empty(void){
    std::lock_guard<std::mutex> lock(m);
    return container.empty();
  }
};

#endif