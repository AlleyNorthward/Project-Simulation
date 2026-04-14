#ifndef OBSERVER_H
#define OBSERVER_H

namespace sdust {

class Subject;

class Observer {
public:
  virtual void update(Subject *subject) = 0;
  virtual ~Observer() {};
};

} // namespace sdust

#endif
