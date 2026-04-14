#ifndef CPUSIMULATIONLIB_H
#define CPUSIMULATIONLIB_H

#include "RegisterChangeStrategy.h"
#include <mutex>

namespace sdust {

class CpuSimulationLib {
protected:
  RegisterChangeStrategy *regStr;
  std::mutex mtx;
  int refresh_interval = 5;

public:
  void run();
  virtual void setRegStr(RegisterChangeStrategy *rs) = 0;
  virtual void configRegister() = 0;
  virtual void refreshTime();
  void setRefeshTime(int time);
  virtual ~CpuSimulationLib() {}
};

} // namespace sdust

#endif
