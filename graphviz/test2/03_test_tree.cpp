#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdint>
using namespace std;

struct AttrMap {
  unordered_map<string,string> m;
  void set(const string &k, const string &v){ m[k]=v; }
  bool empty() const { return m.empty(); }
  string toDot() const {
    if(m.empty()) return "";
    string s; bool first=true;
    for(auto &p: m){
      if(!first) s += ", ";
      first=false;
      s += p.first + "=" + quoteIfNeeded(p.second);
    }
    return s;
  }
  static string quoteIfNeeded(const string &v){
    if (v.size()>0 && v.front()=='{') return v; // raw record
    string out="\"";
    for(char c: v){
      if(c=='\\' || c=='"') out += '\\';
      out += c;
    }
    out += "\"";
    return out;
  }
};

struct Node {
  string id;
  string label;
  AttrMap attrs;
  bool rawLabel = false;
};

struct Edge {
  string from, to;
  AttrMap attrs;
};

class Graph {
  string name="G";
  bool directed=true;
  uint64_t counter=0;
  vector<Node> nodes;
  vector<Edge> edges;
  AttrMap graphAttrs, nodeAttrs, edgeAttrs;

  // generate numeric id part
  string genNumId(){ return to_string(++counter); }
public:
  Graph(string name_="G", bool directed_=true): name(name_), directed(directed_){}

  // --- defaults ---
  Graph &setGraphAttr(const string &k,const string &v){ graphAttrs.set(k,v); return *this; }
  Graph &setNodeDefault(const string &k,const string &v){ nodeAttrs.set(k,v); return *this; }
  Graph &setEdgeDefault(const string &k,const string &v){ edgeAttrs.set(k,v); return *this; }

  // --- add node: 返回 Node& 以便立即设置 node.attrs 等 ---
  Node &addNodeAuto(const string &label, bool raw=false){
    // 给 id 加前缀避免纯数字 id 问题
    string id = string("n") + genNumId();
    nodes.push_back(Node{id,label,AttrMap(),raw});
    return nodes.back();
  }

  // 保留原来的显式 id 版本（返回 Node&）
  Node &addNode(const string &id, const string &label, bool raw=false){
    nodes.push_back(Node{id,label,AttrMap(),raw});
    return nodes.back();
  }

  // add edge（普通）
  void addEdge(const string &from, const string &to){
    edges.push_back(Edge{from,to,AttrMap()});
  }
  // add edge with attrs convenience; 返回 void 或可返回 Edge&（此处简单）
  void addEdge(const string &from, const string &to, const AttrMap &attrs){
    edges.push_back(Edge{from,to,attrs});
  }

  // allow setting defaults conveniently
  Graph &nodeAttr(const string &k,const string &v){ nodeAttrs.set(k,v); return *this; }
  Graph &edgeAttr(const string &k,const string &v){ edgeAttrs.set(k,v); return *this; }

  string toDot() const {
    stringstream ss;
    ss << (directed ? "digraph " : "graph ") << name << " {\n";
    if(!graphAttrs.empty()) ss << "  graph[" << graphAttrs.toDot() << "]\n";
    if(!edgeAttrs.empty()) ss << "  edge[" << edgeAttrs.toDot() << "]\n";
    if(!nodeAttrs.empty()) ss << "  node[" << nodeAttrs.toDot() << "]\n";
    ss << "\n";
    for(const auto &n : nodes){
      ss << "  " << n.id << " [label=";
      if(n.rawLabel) ss << n.label;
      else ss << AttrMap::quoteIfNeeded(n.label);
      if(!n.attrs.empty()) ss << ", " << n.attrs.toDot();
      ss << "]\n";
    }
    ss << "\n";
    string arrow = directed ? " -> " : " -- ";
    for(const auto &e : edges){
      ss << "  " << e.from << arrow << e.to;
      if(!e.attrs.empty()) ss << " [" << e.attrs.toDot() << "]";
      ss << ";\n";
    }
    ss << "}\n";
    return ss.str();
  }

  bool writeToFile(const string &path) const {
    ofstream fout(path);
    if(!fout) return false;
    fout << toDot();
    return true;
  }
};

// ---------- 简单二叉树类型 ----------
struct TreeNode {
  int v;
  TreeNode* left;
  TreeNode* right;
  TreeNode(int _v): v(_v), left(nullptr), right(nullptr) {}
};

// 前序遍历并构建图：返回当前节点的 graph id
string buildFromPreorder(Graph &g, TreeNode* t, const string &parentId = ""){
  if(!t) return "";
  // 创建节点（自动 id），并立即可以设置属性
  Node &n = g.addNodeAuto(to_string(t->v)); // label 为数值字符串
  // 例如设置一些节点特有属性:
  // if (t->v % 2 == 0) n.attrs.set("fillcolor", "#ffeeee");
  string myId = n.id;

  // 若有父节点，连边
  if(!parentId.empty()){
    g.addEdge(parentId, myId);
    // 可传入带属性的边：
    // AttrMap em; em.set("label", "child"); g.addEdge(parentId, myId, em);
  }

  // 递归：前序 -> 左 -> 右
  if(t->left) buildFromPreorder(g, t->left, myId);
  if(t->right) buildFromPreorder(g, t->right, myId);
  return myId;
}

int main(){
  // 构造示例二叉树
  TreeNode *root = new TreeNode(1);
  root->left = new TreeNode(2);
  root->right = new TreeNode(3);
  root->left->left = new TreeNode(4);
  root->left->right = new TreeNode(5);
  root->right->left = new TreeNode(6);

  Graph g("BinaryTree", true);
  // 默认样式（和你原来类似）
  g.setGraphAttr("rankdir", "TB")
   .setNodeDefault("shape", "record")
   .setNodeDefault("fontname", "Consolas")
   .setNodeDefault("fontcolor", "black")
   .setNodeDefault("color", "#8383cc")
   .setNodeDefault("fillcolor", "#b2d3e4")
   .setNodeDefault("style", "filled");

  // 为避免 vector 自动扩容时引用失效（可选）
  // 如果你预计有 N 个节点，先 reserve
  // g.reserveNodes(100); // (如果实现了 reserveNodes)

  // 从根开始构建（前序）
  buildFromPreorder(g, root);

  // 输出 dot
  cout << g.toDot() << endl;
  g.writeToFile("btree.dot");
  cout << "写入 btree.dot 完成\n";

  // 清理内存（示例）
  // ... 省略 delete 链接，实际使用时请负责释放
  return 0;
}












