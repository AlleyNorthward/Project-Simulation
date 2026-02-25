#pragma once

#include "common.h"
#include <vector>

namespace algkit {

class Graph {
private:
  std::string name;
  bool directed;
  int counter;
  std::vector<Node> nodes;
  std::vector<Edge> edges;
  AttrMap graphAttrs;
  AttrMap nodeAttrs;
  AttrMap edgeAttrs;
  std::vector<std::string> infos;
  std::vector<int> layer_count;
  std::string dotPath = "";
  Color_ colorMap;

  std::string genId();
  int sum();

private:
  Graph &setGraphAttr(const std::string &k, const std::string &v);
  Graph &setNodeAttr(const std::string &k, const std::string &v);
  Graph &setEdgeAttr(const std::string &k, const std::string &v);
  Graph &setColorMapAttr(Color color, const std::string &k,
                         const std::string &v);

  void setColor(Color color = Color::Default);
  std::string toDot() const;

public:
  Graph(const std::vector<std::string> &infos_,
        const std::vector<int> &layer_count_, bool directed_ = false,
        std::string name_ = "G");

  void setNodeColor(const std::string &name, Color color);
  std::string addNode(const std::vector<std::string> &values = {},
                      Color color = Color::Default);
  std::string addNode(std::initializer_list<int> values = {},
                      Color color = Color::Default);

  void addEdge(const std::string &from, const std::string &to, int label);
  void addEdge(const std::string &from, const std::string &to,
               const std::string &label);
  void addEdge(const std::string &from, const std::string &to);
  void popEdge();

  bool writeToFile(const std::string &path);
  bool exportSvg(const std::string &path, bool isdelete = false) const;
};
} // namespace algkit
