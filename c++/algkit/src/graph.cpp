#include "graph.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

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

std::string AttrMap::quoteIfNeeded(const std::string &v) {
  std::string out = "\"";
  out += v;
  out += "\"";
  return out;
}

Graph::Graph(const std::vector<std::string> &infos_,
             const std::vector<int> &layer_count_, bool directed_,
             std::string name_)
    : name(std::move(name_)), directed(directed_), counter(0), infos(infos_),
      layer_count(layer_count_) {
  this->setGraphAttr("rankdir", "TB")
      .setEdgeAttr("color", "#8383cc")
      .setEdgeAttr("penwidth", "2")
      .setEdgeAttr("fontcolor", "#9c4a4a")
      .setEdgeAttr("fontsize", "10")
      .setNodeAttr("shape", "record")
      .setNodeAttr("fontname", "Consolas")
      .setNodeAttr("fontcolor", "black")
      .setNodeAttr("color", "#8383cc")
      .setNodeAttr("fillcolor", "#b2d3e4")
      .setNodeAttr("style", "filled")
      .setNodeAttr("penwidth", "2");

  if (!directed)
    this->setEdgeAttr("arrowhead", "none");

  if (this->sum() != static_cast<int>(infos.size())) {
    throw std::invalid_argument(
        "layer_count 和 infos 数量不匹配, 无法继续进行!");
  }
}

Graph &Graph::setGraphAttr(const std::string &k, const std::string &v) {
  graphAttrs.set(k, v);
  return *this;
}

Graph &Graph::setNodeAttr(const std::string &k, const std::string &v) {
  nodeAttrs.set(k, v);
  return *this;
}

Graph &Graph::setColorMapAttr(Color color, std::optional<std::string> k,
                              std::optional<std::string> v) {
  if (color == Color::Default)
    return *this;

  if (k.has_value() && v.has_value()) {
    colorMap[color].set(k.value(), v.value());
  }
  return *this;
}

void Graph::setColor(Color color) {
  auto it = colorMap.find(color);
  if(it != colorMap.end()) return;

  switch (color) {
  case Color::Default:
    // setSingleNodeAttr(color);
    return;
  case Color::Orange:
    this->setColorMapAttr(color, "color", "#ff8c00")
        .setColorMapAttr(color, "fillcolor", "#ffd8a8");
    return;
  case Color::Red:
    this->setColorMapAttr(color, "color", "#c0392b")
        .setColorMapAttr(color, "fillcolor", "#f5b7b1");
    return;
  case Color::Green:
    this->setColorMapAttr(color, "color", "#27ae60")
        .setColorMapAttr(color, "fillcolor", "#abebc6");
    return;
  case Color::Purple:
    this->setColorMapAttr(color, "color", "#8e44ad")
        .setColorMapAttr(color, "fillcolor", "#e8daef");
    return;
  case Color::Teal:
    this->setColorMapAttr(color, "color", "#16a085")
        .setColorMapAttr(color, "fillcolor", "#a3e4d7");
    return;
  case Color::Gray:
    this->setColorMapAttr(color, "color", "#7f8c8d")
        .setColorMapAttr(color, "fillcolor", "#ecf0f1");
    return;
  case Color::Blue:
    this->setColorMapAttr(color, "color", "#1f4e79")
        .setColorMapAttr(color, "fillcolor", "#6f9fc7");
    return;
  default:
    return;
  }
}

void Graph::setNodeColor(const std::string &name, Color color) {
  setColor(color);
  for (auto &n : nodes) {
    if (n.name == name) {
      n.color = color;
      return;
    }
  }

  throw std::invalid_argument("没有找到: " + name);
}

Graph &Graph::setEdgeAttr(const std::string &k, const std::string &v) {
  edgeAttrs.set(k, v);
  return *this;
}

std::string Graph::addNode(std::initializer_list<int> values, Color color) {
  std::vector<std::string> v;
  v.reserve(values.size());

  for (int x : values) {
    v.push_back(std::to_string(x));
  }

  return addNode(v, color);
}

std::string Graph::addNode(const std::vector<std::string> &values,
                           Color color) {
  if (values.size() != infos.size()) {
    throw std::invalid_argument("addNode 参数数量必须和infos一致!");
  }

  int offset = 0;
  std::string id = genId();
  std::stringstream label;
  label << "{step" << id;

  for (size_t i = 0; i < layer_count.size(); i++) {
    label << "|";
    int cnt = layer_count[i];

    label << "{";

    for (int j = 0; j < cnt; j++) {
      if (j != 0)
        label << "|";

      label << infos[offset + j] << ": " << values[offset + j];
    }
    label << "}";
    offset += cnt;
  }

  label << "}";

  setColor(color);

  std::string name = "node" + id;
  nodes.push_back(Node{name, label.str(), color});
  return name;
}

void Graph::addEdge(const std::string &from, const std::string &to) {
  edges.push_back(Edge{from, to});
}

void Graph::addEdge(const std::string &from, const std::string &to, int label) {
  edges.push_back(Edge{from, to, std::to_string(label)});
}

void Graph::addEdge(const std::string &from, const std::string &to,
                    const std::string &label) {
  edges.push_back(Edge{from, to, label});
}

void Graph::popEdge() { edges.pop_back(); }

std::string Graph::toDot() const {
  std::stringstream ss;
  ss << (directed ? "digraph " : "graph ") << name << "{\n";
  if (!graphAttrs.empty())
    ss << "  graph[" << graphAttrs.toDot() << "]\n";
  if (!edgeAttrs.empty())
    ss << "  edge[" << edgeAttrs.toDot() << "]\n";
  if (!nodeAttrs.empty())
    ss << "  node[" << nodeAttrs.toDot() << "]\n";

  ss << "\n";

  for (const auto &n : nodes) {
    ss << "  " << n.name << " [label=";
    ss << AttrMap::quoteIfNeeded(n.label);

    if (n.color != Color::Default) {
      auto it = colorMap.find(n.color);
      if (it != colorMap.end() && !it->second.empty()) {
        ss << ", " << it->second.toDot();
      }
    }

    ss << "]\n";
  }

  ss << "\n";

  std::string arrow = directed ? " -> " : " -- ";
  for (const auto &e : edges) {
    ss << "  " << e.from << arrow << e.to;
    ss << " [label=" << AttrMap::quoteIfNeeded(e.label);
    ss << "];\n";
  }
  ss << "}\n";
  return ss.str();
}

bool Graph::writeToFile(const std::string &path) {
  this->dotPath = path;
  std::ofstream fout(path);
  if (!fout)
    return false;
  fout << toDot();
  return true;
}

bool Graph::exportSvg(const std::string &path, bool isdelete) const {
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

std::string Graph::genId() { return std::to_string(++counter); }

int Graph::sum() {
  int s = 0;
  for (size_t i = 0; i < layer_count.size(); i++) {
    s += layer_count[i];
  }
  return s;
}

} // namespace algkit
