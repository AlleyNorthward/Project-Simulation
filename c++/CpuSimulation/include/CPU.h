#ifndef CPU_H
#define CPU_H

#include "RegisterDescriptorTable.h"
#include "RegisterFile.h"
#include "RegisterView.h"
#include <Subject.h>
#include <vector>

namespace sdust {

class CPU : Subject {
private:
  RegisterFile regs_;
  RegisterDescriptorTable desc_;
  std::vector<Observer *> observers;

public:
  RegisterView view(const std::string &name);
  RegisterDescriptorTable &desc() { return desc_; }
  uint32_t read(const std::string &name) const;
  void write(const std::string &name, uint32_t value);
  std::unordered_map<std::string, uint32_t> dump() const;

  void attach(Observer *o) override;
  void detach(Observer *o) override;
  void notify() override;
};

} // namespace sdust

#endif
