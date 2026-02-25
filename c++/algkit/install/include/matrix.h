#pragma once

#include "common.h"
#include <utility>
#include <vector>

namespace algkit {

class Matrix {
private:
  std::string name;
  int counter;
  std::string dotPath = "";
  AttrMap tableAttrs;
  Color_ colorMap;
  std::vector<Node> nodes;
  int row;
  int col;
  std::vector<std::string> id;
  std::vector<std::string> infos;

private:
  std::string genId();
  Matrix &setTableAttr(const std::string &k, const std::string &v);
  void setColorMapAttr(Color color, const std::string &k, const std::string &v);
  void setColor(Color color);
  std::string toDot() const;

public:
  Matrix(int row_, int col_, Color color_ = Color::Default,
         std::string name_ = "G");
  void setNodeColor(std::pair<int, int> value, Color color);
  void addNode(std::pair<int, int> value, const std::string& info_ = "", Color color = Color::Default);
  bool writeToFile(const std::string &path);
  bool exportSvg(const std::string &path, bool isdelete = false) const;
};
} // namespace algkit
