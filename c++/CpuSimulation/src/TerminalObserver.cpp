#include "TerminalObserver.h"
#include "CPU.h"

namespace sdust {

void TerminalObserver::update(Subject *sub) {
  CPU *cpu = dynamic_cast<CPU *>(sub);
  auto snapshot = cpu->dump();

  for (auto &[k, v] : snapshot) {
    std::cout << k << ": " << v << std::endl;
  }
}

} // namespace sdust
