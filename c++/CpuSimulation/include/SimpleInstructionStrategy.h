#ifndef SIMPLEINSTRUCTIONSTRATEGY_H
#define SIMPLEINSTRUCTIONSTRATEGY_H

#include "RegisterChangeStrategy.h"

namespace sdust {
class SimpleInstructionStrategy : public RegisterChangeStrategy {
public:
  using RegisterChangeStrategy::RegisterChangeStrategy;

  void change() override {
    uint32_t eax = cpu->read("EAX");
    uint32_t ebx = cpu->read("EBX");

    cpu->write("EAX", eax + ebx);
  }
};

} // namespace sdust

#endif
