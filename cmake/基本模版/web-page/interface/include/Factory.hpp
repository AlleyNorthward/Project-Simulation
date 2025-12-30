#ifndef FACTORY_HPP
#define FACTORY_HPP
#include <string>


class Page;
class Link;
class Data;

class Factory{
public:
  virtual ~Factory() = default;
  virtual Page* createPage(const std::string& title, const std::string& author) = 0;
  virtual Link* createLink(const std::string& name, const std::string& url) = 0;
  virtual Data* createData(const std::string& name) = 0;
};

#endif












