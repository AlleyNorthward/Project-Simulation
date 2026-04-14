#ifndef TERMINALOBSERVER_H
#define TERMINALOBSERVER_H

#include "Observer.h"
#include "CPU.h"

namespace sdust{

class TerminalObserver: public Observer{
public:
  void update(Subject* sub);
};

}

#endif












