#ifndef REGISTERDESCRIPTORTABLE_H
#define REGISTERDESCRIPTORTABLE_H

#include "register_base.h"
#include <string>
#include <unordered_map>

namespace sdust {

class RegisterDescriptorTable {
private:
  std::unordered_map<std::string, RegDesc> table;

private:
  static std::string to_upper(std::string s);

public:
  void add(const std::string &name, const std::string &base, Slice slice);
  const RegDesc *find(const std::string &name) const;

  auto begin() const { return table.begin(); }
  auto end() const { return table.end(); }
};

} // namespace sdust

#endif
