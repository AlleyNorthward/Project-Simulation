#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>

class Item{
protected:
  const std::string name;
public:
  Item(const std::string& name);
  virtual ~Item() = default;
  virtual const std::string toHTML() const = 0;
};
#endif












