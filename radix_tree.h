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
        bool is_last = false;
    };
public:
    void insert(std::string_view str);
    void insert_r(std::string_view str);
    [[nodiscard]] std::size_t find(std::string_view str) const;
    [[nodiscard]] std::vector<TreeValue> getAllValues() const;
private:
    NodePtr root_;

    void update_root(std::string root_label, std::string child_label);
    static void insert_recursive(Node* rootNode, char letter, std::string_view str);

    static NodeIter find_node(Node* root, char key);
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
    static Node* insert_node(Node* rootNode, char letter, std::string_view new_label,
            std::string_view old_label, bool is_end);
};




