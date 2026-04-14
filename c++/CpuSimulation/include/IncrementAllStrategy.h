#ifndef INCREMENTALLSTRATEGY_H
#define INCREMENTALLSTRATEGY_H

#include "RegisterChangeStrategy.h"

namespace sdust {

class IncrementAllStrategy : public RegisterChangeStrategy {
public:
  using RegisterChangeStrategy::RegisterChangeStrategy;

  void change() override {
    std::cout << "=== [整体加一模式] ===" << std::endl;
    cpu->beginUpdate();

    auto snapshot = cpu->dump();

    for (auto &[name, value] : snapshot) {
      if (name == "EIP")
        continue;
      cpu->write(name, value + 1);
    }

    uint32_t pc = cpu->read("EIP");
    cpu->write("EIP", pc + 4);

    cpu->endUpdate();
  }
};
} // namespace sdust

#endif
