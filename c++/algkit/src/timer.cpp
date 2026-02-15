#include "timer.h"
#include "iostream"

namespace algkit {
Timer::Timer(const std::string &name)
    : name(name), start(std::chrono::high_resolution_clock::now()) {}

Timer::~Timer() {
  auto end = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration<double, std::milli>(end - start);

  std::cout << name << ": " << duration.count() << "ms" << std::endl;
}
} // namespace algkit
