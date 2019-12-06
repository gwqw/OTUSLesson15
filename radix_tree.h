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
    using NodeIter = LeafContainer::iterator;
    struct Node {
        std::string label_;
        bool is_end_ = true;
        LeafContainer childs;
        Node(std::string label, bool is_end) : label_(std::move(label)), is_end_(is_end) {}
        template <typename...Args>
        void add_node(char key, Args&&... args) {
            childs.emplace(key, std::make_unique<Node>(std::forward<Args>(args)...));
        }
        void add_node(NodePtr &&new_node);
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
    // methods
    static void insert_recursive(NodePtr& rootNode, std::string_view str);
    static std::size_t find_recursive(const NodePtr& rootNode, std::string_view str);
};




