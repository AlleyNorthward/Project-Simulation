#ifndef TERMINALOBSERVER_H
#define TERMINALOBSERVER_H

#include "Observer.h"

namespace sdust {

class TerminalObserver : public Observer {
public:
  void update(const std::unordered_map<std::string, uint32_t> &snapshot);
};

} // namespace sdust

#endif
