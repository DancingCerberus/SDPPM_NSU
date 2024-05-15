#pragma once
#include <cstdint>
#include <stdexcept>
#include <algorithm>

namespace collections {

template<class T>
class CircularBuffer {
  T *body_;
  size_t capacity_;
  size_t size_;
  size_t start_;

 public:
  CircularBuffer() : body_(nullptr), capacity_(0), size_(0), start_(0) {
  }

  explicit CircularBuffer(size_t capacity) : CircularBuffer() {
    if (capacity == 0)
      return;

    body_ = new T[capacity];
    capacity_ = capacity;
  }

  CircularBuffer(const CircularBuffer &cb) : CircularBuffer(cb.capacity_) {
    start_ = cb.start_;

    for (size_t i = 0; i < cb.size_; ++i, ++size_)
      (*this)[i] = cb[i];
  }

  CircularBuffer(size_t size, const T &elem) : CircularBuffer(size) {
    for (; size_ < size; ++size_)
      body_[size_] = elem;
  }

  CircularBuffer(CircularBuffer &&cb) noexcept: body_(nullptr), capacity_(0), size_(0), start_(0) {
      body_ = cb.body_;
      capacity_ = cb.capacity_;
      size_ = cb.size_;
      start_ = cb.start_;

      cb.body_ = nullptr;
  };

  ~CircularBuffer() {
    delete[] body_;
  }

  T &operator[](size_t i) {
    return body_[(start_ + i) % capacity_];
//      return *(body_ + (start_ + i) % capacity_);
  }
  const T &operator[](size_t i) const {
    return body_[(start_ + i) % capacity_];
  }

  T &at(size_t i) {
    if (capacity_ <= i)
      throw std::out_of_range("Try to reach element out of buffer");

    if (size_ <= i)
      throw std::out_of_range("Try to reach uninitialized element");

    return (*this)[i];
  }
  const T &at(size_t i) const {
    if (capacity_ <= i)
      throw std::out_of_range("Try to reach element out of buffer");

    if (size_ <= i)
      throw std::out_of_range("Try to reach uninitialized element");

    return (*this)[i];
  }

  T &front() {
    if (empty())
      throw std::logic_error("Try to reach element from empty buffer");

    return at(0);
  };
  T &back() {
    if (empty())
      throw std::logic_error("Try to reach element from empty buffer");

    return at(size_ - 1);
  };
  const T &front() const {
    if (empty())
      throw std::logic_error("Try to reach element from empty buffer");

    return at(0);
  }
  const T &back() const {
    if (empty())
      throw std::logic_error("Try to reach element from empty buffer");

    return at(size_ - 1);
  }

  T *linearize() {
    T *new_body = new T[capacity_];
    for (size_t i = 0; i < size_; ++i)
      new_body[i] = (*this)[i];

    delete[] body_;

    body_ = new_body;
    start_ = 0;

    return &body_[0];
  }

  bool is_linearized() const {
    return start_ == 0;
  }

  void rotate(size_t new_begin) {
    if (!full())
      throw std::logic_error("Try to rotate buffer that is not full");

    if (size_ <= new_begin)
      throw std::out_of_range("New begin is outside of buffer");

    start_ = new_begin;
  }

  size_t size() const {
    return size_;
  }
  bool empty() const {
    return size_ == 0;
  }

  bool full() const {
    return size() == capacity();
  }

  size_t reserve() const {
    return capacity() - size();
  }

  size_t capacity() const {
    return capacity_;
  }

  void set_capacity(size_t new_capacity) {
    if (new_capacity == capacity_)
      return;
    T *new_body = new T[new_capacity];
    for (size_t i = 0; i < size_; ++i)
      new_body[i] = (*this)[i];

    capacity_ = new_capacity;
    delete[] body_;

    body_ = new_body;
    start_ = 0;
  }

  void resize(size_t new_size, const T &item = T()) {
    if (new_size <= size()) {
      size_ = new_size;
      return;
    }
    if (new_size > capacity())
      set_capacity(new_size);

    for (size_t i = size_; i < new_size; ++i)
      (*this)[size_++] = item;
  }

//  CircularBuffer& operator=(const CircularBuffer &cb) {
  void operator=(const CircularBuffer &cb) {
//    if (this == &cb)
//      return *this;

    if (cb.capacity_ <= capacity_) {
      clear();
    } else {
      delete[] body_;
      body_ = new T[cb.capacity_];
      capacity_ = cb.capacity_;
    }

    for (size_ = 0; size_ < cb.size_; ++size_)
      body_[size_] = cb[size_];

    start_ = 0;

    //return *this;
  };

  CircularBuffer &operator=(CircularBuffer &&cb) noexcept {
    delete[] body_;
    body_ = cb.body_;
    capacity_ = cb.capacity_;
    size_ = cb.size_;
    start_ = cb.start_;

    cb.body_ = nullptr;
  }

  void swap(CircularBuffer &cb) {
    std::swap(body_, cb.body_);
    std::swap(capacity_, cb.capacity_);
    std::swap(size_, cb.size_);
    std::swap(start_, cb.start_);
  }

  void push_back(const T &item = T()) {
    if (capacity_ == 0)
      throw std::out_of_range("Try to add element to a buffer with zero capacity");

    (*this)[size_] = item;
    if (full())
      ++start_;
    else
      ++size_;
  }

  void push_front(const T &item = T()) {
    start_ = start_ == 0 ? capacity_ - 1 : start_ + 1;
    (*this)[0] = item;
    if (!full())
      ++size_;
  }

  void pop_back() {
    if (empty())
      throw std::logic_error("Try delete from empty buffer");
    (*this)[size_ - 1].~T();
    --size_;
  }

  void pop_front() {
    if (empty())
      throw std::logic_error("Try delete from empty buffer");
    (*this)[0].~T();
    --size_;
    start_ = (start_ + 1) % capacity_;
  }

  void insert(size_t pos, const T &item = T()) {
    if (full())
      throw std::logic_error("Try insert item size_to full buffer");

    for (int64_t i = size_ - 1; i >= int64_t(pos); --i)
      (*this)[i + 1] = (*this)[i];

    (*this)[pos] = item;
    ++size_;
  }

  void erase(size_t first, size_t last) {
    if (first == last)
      return;

    for (size_t i = first; i < last; ++i)
      (*this)[i].~T();

    for (size_t i = last; i < size_; ++i)
      (*this)[i - 1] = (*this)[i];

    size_ -= (last - first);
  }

  void clear() {
    for (size_t i = 0; i < size_; ++i)
      (*this)[i].~T();

    size_ = 0;
    start_ = 0;
  }
};

template<class T>
bool operator==(const CircularBuffer<T> &a, const CircularBuffer<T> &b) {
  if (a.size() != b.size())
    return false;

  for (size_t i = 0; i < a.size(); ++i)
    if (a[i] != b[i])
      return false;

  return true;
}

template<class T>
bool operator!=(const CircularBuffer<T> &a, const CircularBuffer<T> &b) {
  return !(a == b);
}
}