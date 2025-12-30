#ifndef DATA_HPP
#define DATA_HPP

#include "Item.hpp"
#include <string>
#include <vector>

class Data: public Item{
protected:
  std::vector<Item *> items;
public:
  Data(const std::string& name);
  virtual ~Data();
  void add(Item* item);
private:
  Data(const Data&) = delete;
  Data& operator=(const Data&) = delete;
  Data(Data &&) = delete;
  Data& operator=(Data &&) = delete;
};

#endif












