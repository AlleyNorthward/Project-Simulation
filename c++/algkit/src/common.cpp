#include "common.h"
namespace algkit {

void AttrMap::set(const std::string &k, const std::string &v) { m[k] = v; }

bool AttrMap::empty() const { return m.empty(); }

std::string AttrMap::toDot() const {
  if (empty())
    return "";
  std::string s;
  bool first = true;

  for (const auto &p : m) {
    if (!first)
      s += ", ";
    first = false;
    s += p.first + "=" + quoteIfNeeded(p.second);
  }
  return s;
}

std::string AttrMap::toHtml() const {
  if (empty())
    return "";

  std::string s;
  bool first = true;

  for (const auto &p : m) {
    if (!first)
      s += " ";
    first = false;

    s += p.first;
    s += "=";
    s += quoteIfNeeded(p.second);
  }

  return s;
}

std::string AttrMap::quoteIfNeeded(const std::string &v) {
  std::string out = "\"";
  out += v;
  out += "\"";
  return out;
}

void Color_::set(Color color, const std::string &k, const std::string &v,
                 bool hasdefault) {
  if (hasdefault)
    return;
  colorMap[color].set(k, v);
}

} // namespace algkit
