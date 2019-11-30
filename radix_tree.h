#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <memory>
#include <utility>
#include <cassert>

class RadixTree {
    struct Node;
    using NodePtr = std::unique_ptr<Node>;
    using LeafContainer = std::unordered_map<char, NodePtr>;
    using NodeIter = std::unordered_map<char, NodePtr>::iterator;
    struct Node {
        std::string label;
        bool is_end = true;
        std::unordered_map<char, NodePtr> childs;
        [[nodiscard]] bool isLeaf() const {return childs.empty();}
    };
public:
    struct TreeValue {
        std::string_view label;
        std::size_t lvl = 0;
        bool is_end = false;
        bool is_leaf = false;
    };
public:
    void insert(std::string_view str);
    [[nodiscard]] std::size_t find(std::string_view str) const;
    [[nodiscard]] std::vector<TreeValue> getAllValues() const;
private:
    NodePtr root_;
    void update_root(std::string root_label, std::string child_label);
    static NodeIter get_closest_node(Node* root, std::string_view str);
    template <typename...Args>
    static Node* add_node(Node* node, char key, Args&&... args) {
        auto [it, res] = node->childs.emplace(key, std::make_unique<Node>(std::forward<Args>(args)...));
        if (res) {
            return it->second.get();
        } else {
            return nullptr;
        }
    }
    static Node* add_node(Node* node, NodePtr&& new_node);
};

using Formater = std::string (*)(const RadixTree::TreeValue& value);

std::string getTreeStructure(const RadixTree& tr, Formater fm);

std::string simple_formater(const RadixTree::TreeValue& value);
std::string pretty_formater(const RadixTree::TreeValue& value);

