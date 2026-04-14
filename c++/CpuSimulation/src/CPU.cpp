#include "CPU.h"
#include "algorithm"

namespace sdust {
RegisterView CPU::view(const std::string &name) {
  return RegisterView(this, name);
}

uint32_t CPU::read(const std::string &name) const {
  const RegDesc *desc = desc_.find(name);

  if (!desc)
    throw std::runtime_error("unknown reg");

  const uint32_t *reg_ptr = regs_.get(desc->base);

  if (!reg_ptr) {
    throw std::runtime_error("base register not initialized");
  }

  uint32_t reg = *reg_ptr;

  uint32_t mask =
      (desc->slice.width == 32) ? 0xFFFFFFFFu : ((1u << desc->slice.width) - 1);

  return (reg >> desc->slice.lsb) & mask;
}

void CPU::write(const std::string &name, uint32_t value) {
  const RegDesc *desc = desc_.find(name);
  if (!desc)
    throw std::runtime_error("unknown reg");

  uint32_t &reg = regs_.get_or_create(desc->base);

  uint32_t mask =
      (desc->slice.width == 32) ? 0xFFFFFFFFu : ((1u << desc->slice.width) - 1);

  uint32_t clearMask = ~(mask << desc->slice.lsb);
  reg = (reg & clearMask) | ((value & mask) << desc->slice.lsb);

  if (isnotify) {
    std::cout << "notify called" << std::endl;
    notify();
    std::cout << "notify called finish" << std::endl;
  }
}

void CPU::beginUpdate() { isnotify = false; }

void CPU::endUpdate() {
  isnotify = true;
  notify();
}

void CPU::attach(Observer *o) { observers.push_back(o); }
void CPU::detach(Observer *o) {
  observers.erase(std::remove(observers.begin(), observers.end(), o),
                  observers.end());
}

void CPU::notify() {
  auto snapshot = regs_.snapshot();
  for (auto *o : observers) {
    o->update(snapshot);
  }
}

} // namespace sdust
