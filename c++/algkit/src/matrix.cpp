#include "matrix.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace algkit {

Matrix::Matrix(int row_, int col_, Color color_, std::string name_)
    : row(row_), col(col_), counter(0), name(std::move(name_)) {
  this->setTableAttr("BORDER", "0")
      .setTableAttr("CELLBORDER", "1")
      .setTableAttr("COLOR", "#8383cc");
  id.resize(row * col, "");
  nodes.resize(row * col, {});
  infos.resize(row * col, "");
}

Matrix &Matrix::setTableAttr(const std::string &k, const std::string &v) {
  tableAttrs.set(k, v);
  return *this;
}

void Matrix::setColorMapAttr(Color color, const std::string &k,
                             const std::string &v) {
  colorMap.set(color, k, v, false);
}

void Matrix::setColor(Color color) {
  auto it = colorMap.colorMap.find(color);
  if (it != colorMap.colorMap.end())
    return;

  switch (color) {
  case Color::Default:
    this->setColorMapAttr(color, "BGCOLOR", "#b2d3e4");
    return;
  case Color::Orange:
    this->setColorMapAttr(color, "BGCOLOR", "#ff8c00");
    return;
  case Color::Red:
    this->setColorMapAttr(color, "BGCOLOR", "#c0392b");
    return;
  case Color::Green:
    this->setColorMapAttr(color, "BGCOLOR", "#27ae60");
    return;
  case Color::Purple:
    this->setColorMapAttr(color, "BGCOLOR", "#8e44ad");
    return;
  case Color::Teal:
    this->setColorMapAttr(color, "BGCOLOR", "#16a085");
    return;
  case Color::Gray:
    this->setColorMapAttr(color, "BGCOLOR", "#7f8c8d");
    return;
  case Color::Blue:
    this->setColorMapAttr(color, "BGCOLOR", "#1f4e79");
    return;
  default:
    this->setColorMapAttr(color, "BGCOLOR", "b2d3e4");
    return;
  }
}

void Matrix::addNode(std::pair<int, int> value, const std::string &info_,
                     Color color) {
  std::string label = "ALIGN=\"CENTER\" VALIGN=\"MIDDLE\" FIXEDSIZE=\"TRUE\" "
                      "WIDTH=\"60\" HEIGHT=\"40\"";
  std::stringstream name;
  name << "(" << std::to_string(value.first) << ", "
       << std::to_string(value.second) << ")";
  setColor(color);

  int index = value.first * col + value.second;
  infos[index] = info_;
  nodes[index] = Node{name.str(), label, color};
}

void Matrix::setNodeColor(std::pair<int, int> value, Color color) {
  int index = value.first * col + value.second;
  id[index] = genId();
  std::stringstream name;
  name << "(" << std::to_string(value.first) << ", "
       << std::to_string(value.second) << ")";

  setColor(color);
  for (auto &n : nodes) {
    if (n.name == name.str()) {
      n.color = color;
      return;
    }
  }
  throw std::invalid_argument("没有找到结点: " + name.str());
}

std::string Matrix::toDot() const {
  std::stringstream ss;
  ss << "digraph " << name << "{\n";
  ss << "  node [shape=record]";
  ss << "  matrix [label=<\n";

  ss << "    <TABLE " << tableAttrs.toHtml() << ">\n";

  for (int i = 0; i < row; ++i) {
    ss << "      <TR>\n";
    for (int j = 0; j < col; ++j) {
      int index = i * col + j;
      ss << "        <TD ";
      ss << nodes[index].label;

      auto it = colorMap.colorMap.find(nodes[index].color);
      if (it != colorMap.colorMap.end() && !it->second.empty()) {
        ss << " " << it->second.toHtml();
      }

      ss << ">";
      ss << infos[index];

      if (id[index] != "") {
        ss << "<BR/>";
        ss << "<FONT COLOR=\"red\" POINT-SIZE=\"8\">";
        ss << id[index];
        ss << "</FONT>";
      }
      ss << "</TD>\n";
    }
    ss << "      </TR>\n";
  }
  ss << "    </TABLE>\n";
  ss << "  >];\n";
  ss << "}\n";

  return ss.str();
}

bool Matrix::writeToFile(const std::string &path) {
  this->dotPath = path;
  std::ofstream fout(path);
  if (!fout)
    return false;
  fout << toDot();
  return true;
}

bool Matrix::exportSvg(const std::string &path, bool isdelete) const {
  if (this->dotPath.empty())
    throw std::invalid_argument("dotPath未设置, 无法生成svg!");

  std::stringstream ss;
  ss << "dot -Tsvg " << this->dotPath << " -o " << path;
  std::string cmd = ss.str();

  int ret = std::system(cmd.c_str());
  if (ret == 0 && isdelete) {
    std::filesystem::remove(this->dotPath);
  }
  return ret == 0;
}

std::string Matrix::genId() { return std::to_string(++counter); }

} // namespace algkit
