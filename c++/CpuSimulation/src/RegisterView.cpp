#include "RegisterView.h"
#include "CPU.h"

namespace sdust {

RegisterView::RegisterView(CPU *cpu_, const std::string &name_)
    : cpu(cpu_), name(name_) {}

uint32_t RegisterView::get() const { return cpu->read(name); }

void RegisterView::set(uint32_t v) { cpu->write(name, v); }

std::ostream &operator<<(std::ostream &os, const RegisterView &r) {
  return os << "0x" << std::hex << r.get() << std::dec;
}

} // namespace sdust
