#pragma once

#include <map>
#include <optional>
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

enum class Color {
  Default,
  Orange,
  Red,
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
  std::map<Color, AttrMap> colorMap;

  std::string genId();
  int sum();

private:
  Graph &setGraphAttr(const std::string &k, const std::string &v);
  Graph &setNodeAttr(const std::string &k, const std::string &v);
  Graph &setEdgeAttr(const std::string &k, const std::string &v);
  Graph &setColorMapAttr(Color color,
                         std::optional<std::string> k = std::nullopt,
                         std::optional<std::string> v = std::nullopt);
  void setColor(Color color = Color::Default);
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

  std::string toDot() const;
  bool writeToFile(const std::string &path);
  bool exportSvg(const std::string &path, bool isdelete = false) const;
};
} // namespace algkit
