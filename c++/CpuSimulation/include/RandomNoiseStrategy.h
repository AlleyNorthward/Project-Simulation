#ifndef RANDOMNOISESTRATEGY_H
#define RANDOMNOISESTRATEGY_H

#include "RegisterChangeStrategy.h"
#include <random>

namespace sdust {

class RandomNoiseStrategy : public RegisterChangeStrategy {
public:
  using RegisterChangeStrategy::RegisterChangeStrategy;

  void change() override {
    std::cout << "=== [随机扰动模式] ===" << std::endl;
    cpu->beginUpdate();

    static std::mt19937 rng{std::random_device{}()};
    static std::uniform_int_distribution<uint32_t> valDist(0, 0xFFFF);

    std::vector<std::string> regs = {"EAX", "EBX", "ECX", "EDX",
                                     "ESI", "EDI", "EBP", "ESP"};

    std::uniform_int_distribution<int> regDist(0, regs.size() - 1);

    std::uniform_int_distribution<int> countDist(1, 3);
    int count = countDist(rng);

    for (int i = 0; i < count; i++) {
      std::string reg = regs[regDist(rng)];
      uint32_t value = valDist(rng);

      cpu->write(reg, value);
    }

    uint32_t pc = cpu->read("EIP");
    cpu->write("EIP", pc + 4);

    cpu->endUpdate();
  }
};
} // namespace sdust

#endif
