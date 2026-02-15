#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace algkit {
struct AttrMap {
  std::unordered_map<std::string, std::string> m;

  void set(const std::string &k, const std::string &v);
  bool empty() const;
  std::string toDot() const;
  static std::string quoteIfNeeded(const std::string &v);
};

struct Node {
  std::string name;
  std::string label;
  AttrMap attrs;
};

struct Edge {
  std::string from, to;
  AttrMap attrs;
};

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

  std::string genId();
  int sum();

public:
  Graph(const std::vector<std::string> &infos_,
        const std::vector<int> &layer_count_, std::string name_ = "G",
        bool directed_ = true);

  Graph &setGraphAttr(const std::string &k, const std::string &v);
  Graph &setNodeAttr(const std::string &k, const std::string &v);
  Graph &setEdgeAttr(const std::string &k, const std::string &v);

  std::string addNode();
  void addEdge(const std::string &from, const std::string &to);
  void addEdge(const std::string &from, const std::string &to,
               const AttrMap &attrs);

  std::string toDot() const;
  bool writeToFile(const std::string &path) const;
};
} // namespace algkit
