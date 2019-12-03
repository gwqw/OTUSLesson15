#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <memory>
#include <utility>
#include <cassert>

class RadixTree {
    struct Node;
    using NodePtr = std::unique_ptr<Node>;
    using LeafContainer = std::array<NodePtr, 256>;
    using NodeIter = LeafContainer::iterator;
    struct Node {
        std::string label_;
        bool is_end_ = true;
        LeafContainer childs;
        Node(std::string label, bool is_end) : label_(std::move(label)), is_end_(is_end) {
            std::fill(childs.begin(), childs.end(), nullptr);
        }
        bool is_node_exist(NodeIter it) const {return it != childs.end() && *it != nullptr;}
        NodeIter find_node(char key) {return childs.begin() + static_cast<unsigned char>(key);}
        template <typename...Args>
        Node* add_node(char key, Args&&... args) {
            childs[(unsigned char)key] = std::make_unique<Node>(std::forward<Args>(args)...);
            return childs[(unsigned char)key].get();
        }
        Node* add_node(NodePtr &&new_node);
        Node* insert_node(char letter, std::string_view new_label,
                          std::string_view old_label, bool is_end);
    };
public:
    struct TreeValue {
        std::string_view label;
        std::size_t lvl = 0;
        bool is_end = false;
        bool is_last = false;
    };
public:
    void insert(std::string_view str);
    [[nodiscard]] std::size_t find(std::string_view str) const;
    [[nodiscard]] std::vector<TreeValue> getAllValues() const;
private:
    NodePtr root_;

    void update_root(std::string root_label, std::string child_label);
    static void insert_recursive(Node* rootNode, char letter, std::string_view str);
};




