#ifndef CLOCKTICKSTRATEGY_H
#define CLOCKTICKSTRATEGY_H

#include "RegisterChangeStrategy.h"
namespace sdust {
class ClockTickStrategy : public RegisterChangeStrategy {
public:
  using RegisterChangeStrategy::RegisterChangeStrategy;

  void change() override {
    uint32_t pc = cpu->read("PC");
    cpu->write("PC", pc + 4);
  }
};
} // namespace sdust

#endif
