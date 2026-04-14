#ifndef MOVESTRATEGY_H
#define MOVESTRATEGY_H

#include "RegisterChangeStrategy.h"

namespace sdust {

class MoveStrategy : public RegisterChangeStrategy {
public:
  using RegisterChangeStrategy::RegisterChangeStrategy;
  void change() {
    std::cout << "=== [mov模式] ===" << std::endl;
    cpu->beginUpdate();
    uint32_t ecx = cpu->read("ECX");
    cpu->write("EDI", ecx);

    uint32_t pc = cpu->read("EIP");
    cpu->write("EIP", pc + 4);

    cpu->endUpdate();
  }
};

} // namespace sdust

#endif
