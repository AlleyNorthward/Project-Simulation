#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H

#include <string>
#include <unordered_map>

namespace sdust {

class RegisterFile {
private:
  std::unordered_map<std::string, uint32_t> reg;

public:
  uint32_t &get_or_create(const std::string &name) { return reg[name]; }
  std::unordered_map<std::string, uint32_t> snapshot() const { return reg; }
};

} // namespace sdust

#endif
