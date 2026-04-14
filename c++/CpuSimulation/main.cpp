#include "CpuApplication.h"
#include "CpuSimulationLib.h"

int main() {
  sdust::CpuSimulationLib *cpu = new sdust::CpuApplication;
  cpu->run();

  delete cpu;
}
