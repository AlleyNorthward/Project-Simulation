#include "CpuSimulationLib.h"
#include <chrono>
#include <thread>

namespace sdust {

void CpuSimulationLib::run() {
  configRegister();

  while (1) {
    {
      std::lock_guard<std::mutex> lock(mtx);
      regStr->change();
    }
    refreshTime();
  }
}

void CpuSimulationLib::refreshTime() {
  std::this_thread::sleep_for(std::chrono::seconds(refresh_interval));
}

void CpuSimulationLib::setRefeshTime(int time) { refresh_interval = time; }

} // namespace sdust
