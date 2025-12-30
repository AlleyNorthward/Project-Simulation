#include "Data.hpp"

Data::Data(const std::string &name) : Item(name) {}
Data::~Data() { items.clear(); }
void Data::add(Item *item) { items.push_back(item); }
