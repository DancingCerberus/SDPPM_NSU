#include "TestUtils.h"
using namespace collections;


bool isEqual(const CircularBuffer<int> &cb, const std::vector<int> &vec) {
  if (cb.size() != vec.size())
    return false;

  for (size_t i = 0; i < cb.size(); ++i)
    if (cb[i] != vec[i])
      return false;

  return true;
}