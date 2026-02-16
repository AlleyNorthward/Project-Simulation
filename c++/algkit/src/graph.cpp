#include "graph.h"
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
             const std::vector<int> &layer_count_, std::string name_,
             bool directed_)
    : name(std::move(name_)), directed(directed_), counter(0), infos(infos_),
      layer_count(layer_count_) {
  this->setGraphAttr("rankdir", "TB")
      .setEdgeAttr("color", "#8383cc")
      .setEdgeAttr("penwidth", "2")
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

Graph &Graph::setEdgeAttr(const std::string &k, const std::string &v) {
  edgeAttrs.set(k, v);
  return *this;
}

std::string Graph::addNode(const std::vector<std::string> &values) {
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

  std::string name = "node" + id;
  nodes.push_back(Node{name, label.str(), AttrMap()});
  return name;
}

void Graph::addEdge(const std::string &from, const std::string &to) {
  edges.push_back(Edge{from, to, AttrMap()});
}

void Graph::addEdge(const std::string &from, const std::string &to,
                    const AttrMap &attrs) {
  edges.push_back(Edge{from, to, attrs});
}

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
    if (!n.attrs.empty())
      ss << ", " << n.attrs.toDot();
    ss << "]\n";
  }

  ss << "\n";

  std::string arrow = directed ? " -> " : " -- ";
  for (const auto &e : edges) {
    ss << "  " << e.from << arrow << e.to;
    if (!e.attrs.empty())
      ss << "[" << e.attrs.toDot() << "]";
    ss << ";\n";
  }
  ss << "}\n";
  return ss.str();
}

bool Graph::writeToFile(const std::string &path) const {
  std::ofstream fout(path);
  if (!fout)
    return false;
  fout << toDot();
  return true;
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
