#pragma once

#include <map>
#include <string>
#include <unordered_map>

namespace algkit {
struct AttrMap {
  std::unordered_map<std::string, std::string> m;

  void set(const std::string &k, const std::string &v);
  bool empty() const;
  std::string toDot() const;
  std::string toHtml() const;
  static std::string quoteIfNeeded(const std::string &v);
};

enum class Color { Default, Orange, Red, Green, Purple, Teal, Gray, Blue };

struct Color_ {
  std::map<Color, AttrMap> colorMap;
  void set(Color color, const std::string &k, const std::string &v, bool hasdefault = true);
};

struct Node {
  std::string name;
  std::string label;
  Color color;
};

struct Edge {
  std::string from;
  std::string to;
  std::string label;
};

} // namespace algkit
