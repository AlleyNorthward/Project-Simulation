#ifndef REGISTERCHANGESTRATEGY_H
#define REGISTERCHANGESTRATEGY_H

#include "CPU.h"

namespace sdust {

class RegisterChangeStrategy {
protected:
  CPU *cpu;

public:
  RegisterChangeStrategy(CPU *cpu_) : cpu(cpu_) {}
  virtual void change() = 0;
  virtual ~RegisterChangeStrategy() {};
};

} // namespace sdust

#endif
