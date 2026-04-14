#ifndef REGISTERVIEW_H
#define REGISTERVIEW_H

#include <iostream>
#include <cstdint>

namespace sdust {

class CPU;

class RegisterView {
private:
  CPU *cpu;
  std::string name;

public:
  RegisterView(CPU *cpu_, const std::string &name_);
  uint32_t get() const;
  void set(uint32_t v);
  friend std::ostream &operator<<(std::ostream &, const RegisterView &r);
};

} // namespace sdust

#endif
