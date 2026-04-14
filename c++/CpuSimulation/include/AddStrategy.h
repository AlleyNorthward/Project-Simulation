#ifndef ADDSTRATEGY_H
#define ADDSTRATEGY_H

#include "RegisterChangeStrategy.h"

namespace sdust {

class AddStrategy : public RegisterChangeStrategy {
public:
  using RegisterChangeStrategy::RegisterChangeStrategy;
  void change() {
    std::cout << "=== [添加模式] ===" << std::endl;
    cpu->beginUpdate();

    uint32_t eax = cpu->read("EAX");
    uint32_t ebx = cpu->read("EBX");

    uint32_t ecx = cpu->read("ECX");
    uint32_t edx = cpu->read("EDX");

    cpu->write("EAX", eax + ebx);
    cpu->write("ECX", ecx + edx);

    uint32_t pc = cpu->read("EIP");
    cpu->write("EIP", pc + 4);

    cpu->endUpdate();
  }
};

} // namespace sdust

#endif
