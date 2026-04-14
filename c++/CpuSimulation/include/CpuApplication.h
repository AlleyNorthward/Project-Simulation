#ifndef CPUAPPLICATION_H
#define CPUAPPLICATION_H

#include <CpuSimulationLib.h>
#include <CPU.h>
#include "TerminalObserver.h"

namespace sdust {

class CpuApplication : public CpuSimulationLib {
private:
  CPU* cpu;
  TerminalObserver* to;

  RegisterChangeStrategy* iStra;
  RegisterChangeStrategy* aStra;
  RegisterChangeStrategy* mStra;
  RegisterChangeStrategy* rStra;

  std::vector<RegisterChangeStrategy*> strategies;
  int idx;

public:
  void initCpu();
  void configRegister();
  void changeRegStr();
  ~CpuApplication();
};

} // namespace sdust

#endif
