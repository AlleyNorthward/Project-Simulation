#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

struct AttrMap {
  std::unordered_map<std::string, std::string> m;
  void set(const std::string &k, const std::string &v) { m[k] = v; }
  bool empty() const { return m.empty(); }

  std::string toDot() const {
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

  static std::string quoteIfNeeded(const std::string &v) {
    /*
       思考一下, 这里函数返回类型是string类型, out是栈上的临时变量,
       是否可以正确返回呢? 如果返回的是引用, 那么这种返回方式是否正确呢?

       思考一个问题, 如果返回值为某引用类型, 既然无法返回栈上变量, 那么是否说明,
       函数一定会传参, 且为相同类型的引用或指针呢? 大部分情况下是正确的,
       但也存在例外. 比如返回静态类型引用(单例模式), 或者是类中this指针,
       应该也可以.
    */
    std::string out = "\"";
    out += v;
    out += "\"";
    return out;
  }
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
  std::string name = "G";
  bool directed = true;
  int counter = 0;
  std::vector<Node> nodes;
  std::vector<Edge> edges;
  AttrMap graphAttrs;
  AttrMap nodeAttrs;
  AttrMap edgeAttrs;
  std::vector<std::string> infos;
  std::vector<std::string> values;
  std::vector<int> layer_count;

public:
  Graph(const std::vector<std::string> &infos_,
        const std::vector<int> layer_count_, std::string name_ = "G",
        bool directed_ = true)
      : infos(infos_), layer_count(layer_count_), name(std::move(name_)),
        directed(directed_) {
    this->setGraphAttr("rankdir", "TB")
        .setEdgeAttr("arrowhead", "none")
        .setEdgeAttr("color", "#8383cc")
        .setEdgeAttr("penwidth", "2")
        .setNodeAttr("shape", "record")
        .setNodeAttr("fontname", "Consolas")
        .setNodeAttr("fontcolor", "black")
        .setNodeAttr("color", "#8383cc")
        .setNodeAttr("fillcolor", "#b2d3e4")
        .setNodeAttr("style", "filled")
        .setNodeAttr("penwidth", "2");

    if (this->sum() != infos.size()) {
      throw std::invalid_argument(
          "layer_count 和 infos 数量不匹配, 无法继续进行!");
    }

    for (int i = 0; i < infos.size(); i++) {
      values.push_back("None");
    }
  }

  Graph &setGraphAttr(const std::string &k, const std::string &v) {
    graphAttrs.set(k, v);
    return *this;
  }

  Graph &setNodeAttr(const std::string &k, const std::string &v) {
    nodeAttrs.set(k, v);
    return *this;
  }

  Graph &setEdgeAttr(const std::string &k, const std::string &v) {
    edgeAttrs.set(k, v);
    return *this;
  }

  std::string addNode() {
    int offset = 0;
    std::string id = genId();
    std::stringstream label;
    label << "{step" << id;

    for (int i = 0; i < layer_count.size(); i++) {
      label << "|";
      int cnt = layer_count[i];

      label << "{";

      for (int j = 0; j < cnt; j++) {
        if (j != 0)
          label << "|";

        label << infos[offset + j] << " " << values[offset + j];
      }
      label << "}";
      offset += cnt;
    }

    label << "}";

    std::string name = "node" + id;
    nodes.push_back(Node{name, label.str(), AttrMap()});
    return name;
  }

  void setValue(const std::vector<std::string> &v) { values = v; }
  // void setValue(std::vector<std::string> v) { values = std::move(v); }

  void addEdge(const std::string &from, const std::string &to) {
    edges.push_back(Edge{from, to, AttrMap()});
  }

  void addEdge(const std::string &from, const std::string &to,
               const AttrMap &attrs) {
    edges.push_back(Edge{from, to, attrs});
  }

  std::string toDot() const {
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
    for (auto &e : edges) {
      ss << "  " << e.from << arrow << e.to;
      if (!e.attrs.empty())
        ss << "[" << e.attrs.toDot() << "]";
      ss << ";\n";
    }
    ss << "}\n";
    return ss.str();
  }

  bool writeToFile(const std::string &path) const {
    std::ofstream fout(path);
    if (!fout)
      return false;
    fout << toDot();
    return true;
  }

private:
  std::string genId() { return std::to_string(++counter); }
  int sum() {
    int s = 0;
    for (int i = 0; i < layer_count.size(); i++) {
      s += layer_count[i];
    }
    return s;
  }
};

struct TreeNode {
  int left = 0;
  int right = 0;
};

class BinaryTree {
public:
  BinaryTree(int n) : n(n), nodes(n + 1) {

    std::vector<std::string> infos{"查看传入数值"};
    std::vector<int> layer_count{1};
    try {
      g = new Graph(infos, layer_count);
    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
      std::exit(-1);
    }
  }

  ~BinaryTree() { delete g; }

  void setChild(int idx, int l, int r) {
    nodes[idx].left = l;
    nodes[idx].right = r;
  }

  void preorder(int u, std::vector<int> &out, const std::string &parent) {
    std::vector<std::string> uu = {std::to_string(u)};
    this->graph().setValue(uu);
    std::string children = g->addNode();
    if (!parent.empty())
      g->addEdge(parent, children);

    if (u == 0)
      return;
    out.push_back(u);
    preorder(nodes[u].left, out, children);
    preorder(nodes[u].right, out, children);
  }

  void inorder(int u, std::vector<int> &out) {
    if (u == 0)
      return;
    inorder(nodes[u].left, out);
    out.push_back(u);
    inorder(nodes[u].right, out);
  }

  void postorder(int u, std::vector<int> &out) {
    if (u == 0)
      return;
    postorder(nodes[u].left, out);
    postorder(nodes[u].right, out);
    out.push_back(u);
  }

  Graph &graph() const { return *g; }

private:
  int n;
  std::vector<TreeNode> nodes;
  Graph *g;
};

int main() {
  int n;
  std::cin >> n;

  BinaryTree bt(n);

  for (int i = 1; i <= n; i++) {
    int l, r;
    std::cin >> l >> r;
    bt.setChild(i, l, r);
  }

  std::vector<int> pre, in, post;
  bt.preorder(1, pre, "");

  for (int x : pre)
    std::cout << x << " ";
  std::cout << "\n";
  std::string dot = bt.graph().toDot();

  if (!bt.graph().writeToFile("example.dot")) {
    std::cerr << "写入 example.dot 失败\n";
  } else {
    std::cout << "写入 example.dot 成功\n";
  }
  return 0;
}
