#include "TerminalObserver.h"
#include <iomanip>
#include <iostream>

namespace sdust {

void TerminalObserver::update(
    const std::unordered_map<std::string, uint32_t> &snapshot) {
  std::cout << "==== [CPU STATE] ====" << std::endl;

  for (const auto &[name, value] : snapshot) {
    std::cout << "[" << name << "]: 0x" << std::setw(8) << std::setfill('0')
              << std::hex << value << std::dec << std::endl;
  }
  std::cout << std::endl;
}

} // namespace sdust
