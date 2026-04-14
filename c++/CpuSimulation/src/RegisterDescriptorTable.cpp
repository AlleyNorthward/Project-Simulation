#include "RegisterDescriptorTable.h"
#include <cctype>

namespace sdust {

std::string RegisterDescriptorTable::to_upper(std::string s) {
  for (auto &c : s)
    c = std::toupper((unsigned char)c);
  return s;
}

void RegisterDescriptorTable::add(const std::string &name,
                                  const std::string &base, Slice slice) {
  table[to_upper(name)] = {name, base, slice};
}

const RegDesc *RegisterDescriptorTable::find(const std::string &name) const {
  auto it = table.find(to_upper(name));
  if (it == table.end())
    return nullptr;
  return &it->second;
}

} // namespace sdust
