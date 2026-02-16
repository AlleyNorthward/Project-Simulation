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
};

struct Edge {
  std::string from;
  std::string to;
  std::string label;
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
  std::string dotPath = "";

  std::string genId();
  int sum();

public:
  Graph(const std::vector<std::string> &infos_,
        const std::vector<int> &layer_count_, bool directed_ = false,
        std::string name_ = "G");

  Graph &setGraphAttr(const std::string &k, const std::string &v);
  Graph &setNodeAttr(const std::string &k, const std::string &v);
  Graph &setEdgeAttr(const std::string &k, const std::string &v);

  std::string addNode(const std::vector<std::string> &values = {});
  std::string addNode(std::initializer_list<int> values = {});

  void addEdge(const std::string &from, const std::string &to, int label);
  void addEdge(const std::string &from, const std::string &to,
               const std::string &label);
  void addEdge(const std::string &from, const std::string &to);
  void popEdge();

  std::string toDot() const;
  bool writeToFile(const std::string &path);
  bool exportSvg(const std::string &path) const;
};
} // namespace algkit
