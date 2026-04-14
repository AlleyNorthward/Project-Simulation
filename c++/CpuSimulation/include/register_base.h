#ifndef REGISTER_BASE_H
#define REGISTER_BASE_H

#include <cstdint>
#include <string>

namespace sdust {

struct Register32 {
  uint32_t value = 0;
};

struct Slice {
  uint8_t lsb;
  uint8_t width;
};

struct RegDesc {
  std::string name;
  std::string base;
  Slice slice;
};

} // namespace sdust

#endif
