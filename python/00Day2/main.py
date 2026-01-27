import subprocess

STEP = 0

class Node:
    def __init__(self, id, label):
        self.id = id
        self.label = label
        self.state = "normal"

class TreeState:
    def __init__(self):
        self.nodes = {}
        self.edges = []

    def add_node(self, node):
        self.nodes[node.id] = node

    def add_edge(self, u, v):
        self.edges.append((u, v))

def emit(tree_state, note=""):
    global STEP
    STEP += 1

    dot = ["digraph G {", "  rankdir=TB;"]

    for n in tree_state.nodes.values():
        color = "black"
        if n.state == "visited":
            color = "orange"

        dot.append(f'  {n.id} [label="{n.label}", color="{color}", fontcolor="{color}"];')

    for u, v in tree_state.edges:
        dot.append(f"  {u} -> {v};")

    dot.append("}")

    dot_text = "\n".join(dot)

    dot_path = f"step_{STEP:03d}.dot"
    with open(dot_path, "w", encoding="utf8") as f:
        f.write(dot_text)

    svg_path = f"step_{STEP:03d}.svg"
    subprocess.run(["dot", "-Tsvg", dot_path, "-o", svg_path])

    print(f"Generated {svg_path}: {note}")


def build_tree():
    t = TreeState()
    t.add_node(Node(1, "1"))
    t.add_node(Node(2, "2"))
    t.add_node(Node(3, "3"))
    t.add_edge(1, 2)
    t.add_edge(1, 3)
    return t

def dfs(node_id, tree):
    node = tree.nodes[node_id]
    node.state = "visited"
    emit(tree, note=f"visit {node.label}")

    if node_id == 1:
        dfs(2, tree)
        dfs(3, tree)

tree = build_tree()
emit(tree, "initial state")
dfs(1, tree)
emit(tree, "done")
