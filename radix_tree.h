#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <list>
#include <memory>
#include <optional>

class RadixTree {
    struct Node;
    using  NodePtr = std::unique_ptr<Node>;
    struct Node {
        std::string label;
        bool is_end = true;
        std::list<NodePtr> childs;
        [[nodiscard]] bool isLeaf() const {return childs.empty();}
    };

    struct TreeValue {
        std::string_view label;
        std::size_t lvl = 0;
        bool is_end = false;
    };
    template <typename...Args>
    static Node* add_node(Node* node, Args&&... args) {
        node->childs.push_back(std::make_unique<Node>(std::forward<Args>(args)...));
        return node->childs.back().get();
    }
    static Node* add_node(Node* node, NodePtr&& new_node);
public:
    void insert(std::string_view str);
    [[nodiscard]] std::optional<std::string_view> find(std::string_view str) const;
    [[nodiscard]] std::vector<TreeValue> getAllValues() const;
private:
    NodePtr root_;
    void update_root(std::string root_label, std::string child_label);
};

std::string getTreeStructure(const RadixTree& tr);


