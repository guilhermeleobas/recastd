#pragma once

#include <vector>

using std::vector;

typedef unsigned int uint;

template <class T>
class small_queue : public vector<T> {
 private:
  uint max_len;

 public:
  small_queue() : max_len(0) {}
  small_queue(const uint max_len) : max_len(max_len) { this->reserve(max_len + 10); }
  void push(const T& t) {
    while (this->size() >= this->max_len){
      this->pop_back();
    }
    
    this->insert(this->begin(), t);
  }
  
  uint get_max_len() const { return this->max_len; }
};
