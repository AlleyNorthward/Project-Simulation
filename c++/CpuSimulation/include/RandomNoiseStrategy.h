#ifndef RANDOMNOISESTRATEGY_H
#define RANDOMNOISESTRATEGY_H

#include <random>
#include "RegisterChangeStrategy.h"

namespace sdust {

class RandomNoiseStrategy : public RegisterChangeStrategy {
public:
  using RegisterChangeStrategy::RegisterChangeStrategy;

  void change() override {
    static std::mt19937 rng{std::random_device{}()};
    static std::uniform_int_distribution<uint32_t> dist(0, 10);

    auto snapshot = cpu->dump();

    for (auto &[name, value] : snapshot) {
      cpu->write(name, value + dist(rng));
    }
  }
};
} // namespace sdust

#endif
