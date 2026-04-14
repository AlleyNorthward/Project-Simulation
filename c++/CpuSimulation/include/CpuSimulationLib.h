#ifndef CPUSIMULATIONLIB_H
#define CPUSIMULATIONLIB_H

#include "RegisterChangeStrategy.h"

namespace sdust {

class CpuSimulationLib {
protected:
  RegisterChangeStrategy *regStr = nullptr;
  int refresh_interval = 5;

public:
  void run();
  virtual void initCpu() = 0;
  virtual void configRegister() = 0;
  virtual void changeRegStr() = 0;
  virtual void refreshTime();
  void setRefeshTime(int time);
  virtual ~CpuSimulationLib() {}
};

} // namespace sdust

#endif
