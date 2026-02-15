#include "graph.h"
int main() {
  std::vector<std::string> infos{"装备量", "粮食量", "信息量"};
  std::vector<int> layer_count = {2, 1};
  Graph g{infos, layer_count};

  std::string root = g.addNode();
  std::string children = g.addNode();
  std::string children_ = g.addNode();
  g.addEdge(root, children);
  g.addEdge(root, children_);

  std::string dot = g.toDot();
  g.writeToFile("example.dot");
  return 0;
}
