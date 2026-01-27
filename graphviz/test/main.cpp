#include <iostream>
#include <fstream>
#include <vector>
#include <string>

struct TreeNode {
    std::string name;
    std::vector<TreeNode*> children;

    TreeNode(const std::string& n) : name(n) {}

    void addChild(TreeNode* child) {
        children.push_back(child);
    }
};

class DotGenerator {
public:
    static void generate(TreeNode* root, const std::string& filename) {
        std::ofstream out(filename);
        out << "digraph Tree {\n";
        out << "    node [shape=box];\n";

        generateNode(out, root);

        out << "}\n";
        out.close();
    }

private:
    static void generateNode(std::ofstream& out, TreeNode* node) {
        out << "    \"" << node->name << "\";\n";

        for (auto child : node->children) {
            out << "    \"" << node->name << "\" -> \"" << child->name << "\";\n";
            generateNode(out, child);
        }
    }
};

class GraphvizRunner {
public:
    static void render(const std::string& dotFile, const std::string& outFile) {
        std::string cmd = "dot -Tpng " + dotFile + " -o " + outFile;
        system(cmd.c_str());
    }
};

int main() {
    TreeNode* root = new TreeNode("Root");
    TreeNode* child1 = new TreeNode("Child1");
    TreeNode* child2 = new TreeNode("Child2");
    TreeNode* grandChild = new TreeNode("GrandChild");

    root->addChild(child1);
    root->addChild(child2);
    child1->addChild(grandChild);

    DotGenerator::generate(root, "tree.dot");

    GraphvizRunner::render("tree.dot", "tree.png");

    std::cout << "Done: tree.png" << std::endl;
    return 0;
}
