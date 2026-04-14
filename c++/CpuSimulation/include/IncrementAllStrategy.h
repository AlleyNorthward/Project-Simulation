#ifndef INCREMENTALLSTRATEGY_H
#define INCREMENTALLSTRATEGY_H

#include "RegisterChangeStrategy.h"

namespace sdust {

class IncrementAllStrategy : public RegisterChangeStrategy {
public:
  using RegisterChangeStrategy::RegisterChangeStrategy;

  void change() override {
    auto snapshot = cpu->dump();

    for (auto &[name, value] : snapshot) {
      cpu->write(name, value + 1);
    }
  }
};
} // namespace sdust

#endif
