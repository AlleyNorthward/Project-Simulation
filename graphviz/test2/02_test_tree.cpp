#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdint>
#include <cstdlib>
using namespace std;

struct AttrMap {
  unordered_map<string, string> m;
  void set(const string &k, const string &v) { m[k] = v; }
  bool empty() const { return m.empty(); }
  string toDot() const {
    if (m.empty())
      return "";
    string s;
    bool first = true;
    for (auto &p : m) {
      if (!first)
        s += ", ";
      first = false;
      s += p.first + "=" + quoteIfNeeded(p.second);
    }
    return s;
  }
  static string quoteIfNeeded(const string &v) {
    string out = "\"";
    out += v;
    out += "\"";
    return out;
  }
};

struct Node {
  string id;
  string label;
  AttrMap attrs;
};

struct Edge {
  string from, to;
  AttrMap attrs;
};

class Graph {
  string name = "G";
  bool directed = true;
  uint64_t counter = 0;
  vector<Node> nodes;
  vector<Edge> edges;
  AttrMap graphAttrs;
  AttrMap nodeAttrs;
  AttrMap edgeAttrs;
  // 小工具：生成唯一 id
  string genId() { return to_string(++counter); }

public:
  Graph(string name_ = "G", bool directed_ = true)
      : name(name_), directed(directed_) {}

  // 设置默认属性
  Graph &setGraphAttr(const string &k, const string &v) {
    graphAttrs.set(k, v);
    return *this;
  }
  Graph &setNodeDefault(const string &k, const string &v) {
    nodeAttrs.set(k, v);
    return *this;
  }
  Graph &setEdgeDefault(const string &k, const string &v) {
    edgeAttrs.set(k, v);
    return *this;
  }

  // 添加节点（显式 id）
  Node &addNode(const string &id, const string &label) {
    nodes.push_back(Node{id, label, AttrMap()});
    return nodes.back();
  }
  // 添加节点（自动 id）
  string addNodeAuto(const string &label) {
    string id = genId();
    nodes.push_back(Node{id, label, AttrMap()});
    return id;
  }

  // 添加边
  void addEdge(const string &from, const string &to) {
    edges.push_back(Edge{from, to, AttrMap()});
  }
  void addEdge(const string &from, const string &to, const AttrMap &attrs) {
    edges.push_back(Edge{from, to, attrs});
  }

  // 链式快捷：返回 this
  Graph &nodeAttr(const string &k, const string &v) {
    nodeAttrs.set(k, v);
    return *this;
  }
  Graph &edgeAttr(const string &k, const string &v) {
    edgeAttrs.set(k, v);
    return *this;
  }

  // 导出为 dot 文本
  string toDot() const {
    stringstream ss;
    ss << (directed ? "digraph " : "graph ") << name << " {\n";
    // graph defaults
    if (!graphAttrs.empty())
      ss << "  graph[" << graphAttrs.toDot() << "]\n";
    if (!edgeAttrs.empty())
      ss << "  edge[" << edgeAttrs.toDot() << "]\n";
    if (!nodeAttrs.empty())
      ss << "  node[" << nodeAttrs.toDot() << "]\n";
    ss << "\n";
    // nodes
    for (auto &n : nodes) {
      ss << "  " << n.id << " [label=";
      ss << AttrMap::quoteIfNeeded(n.label);
      // node-specific attrs
      if (!n.attrs.empty())
        ss << ", " << n.attrs.toDot();
      ss << "]\n";
    }
    ss << "\n";
    // edges
    string arrow = directed ? " -> " : " -- ";
    for (auto &e : edges) {
      ss << "  " << e.from << arrow << e.to;
      if (!e.attrs.empty())
        ss << "[" << e.attrs.toDot() << "]";
      ss << ";\n";
    }
    ss << "}\n";
    return ss.str();
  }

  // 写文件
  bool writeToFile(const string &path) const {
    ofstream fout(path);
    if (!fout)
      return false;
    fout << toDot();
    return true;
  }

  // 调用系统的 dot 渲染（需要系统安装 graphviz）
  // 注意：为示例简单实现，生产环境请更健壮地处理安全/错误/临时文件
  bool renderWithGraphviz(const string &outPath,
                          const string &format = "png") const {
    string tmpDot = "/tmp/graph_temp_" + to_string((uintptr_t)this) + ".dot";
    if (!writeToFile(tmpDot))
      return false;
    string cmd = "dot -T" + format + " " + tmpDot + " -o " + outPath;
    int rc = system(cmd.c_str());
    // remove tmp? 可以保留以便调试
    // remove(tmpDot.c_str());
    return rc == 0;
  }
};

// ---------- 示例用法 ----------
int main() {
  Graph g("G", true);

  // 设置默认样式（和用户示例一致）
  g.setGraphAttr("rankdir", "TB")
      .edgeAttr("arrowhead", "none")
      .edgeAttr("color", "#8383cc")
      .edgeAttr("penwidth", "2")
      .setNodeDefault("shape", "record")
      .setNodeDefault("fontname", "Consolas")
      .setNodeDefault("fontcolor", "black")
      .setNodeDefault("color", "#8383cc")
      .setNodeDefault("fillcolor", "#b2d3e4")
      .setNodeDefault("style", "filled")
      .setNodeDefault("penwidth", "2");

  // 添加节点（用户示例使用 record 标签，这里用 rawLabel=true 保持原样）
  g.addNode("1", "{step1|{装备量|粮食量|信息量}}");
  g.addNode("2", "{step2|{装备量|粮食量|信息量}}");
  g.addNode("3", "{step3|{装备量|粮食量|信息量}}");

  // 添加边
  g.addEdge("1", "2");
  g.addEdge("1", "3");

  // 导出 dot
  string dot = g.toDot();
  cout << dot << endl; // 打印到控制台

  // 写文件
  if (!g.writeToFile("example.dot")) {
    cerr << "写入 example.dot 失败\n";
  } else {
    cout << "写入 example.dot 成功\n";
  }

  // 可选：调用系统 dot 渲染（需要 graphviz 已安装）
  // g.renderWithGraphviz("example.png", "png");

  return 0;
}
