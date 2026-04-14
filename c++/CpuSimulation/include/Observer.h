#ifndef OBSERVER_H
#define OBSERVER_H
#include <cstdint>
#include <string>
#include <unordered_map>

namespace sdust {

class Subject;

class Observer {
public:
  virtual void
  update(const std::unordered_map<std::string, uint32_t> &snapshot) = 0;
  virtual ~Observer() {};
};

} // namespace sdust

#endif
