#pragma once
#include <chrono>
#include <string>

namespace algkit {
class Timer {
public:
  Timer(const std::string &name = "");
  ~Timer();

public:
  std::string name;
  std::chrono::high_resolution_clock::time_point start;
};
} // namespace algkit
