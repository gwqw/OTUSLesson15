#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <memory>

class RadixTree {
    struct Node;
    using  NodePtr = std::unique_ptr<Node>;
    struct Node {
        std::string label;
        bool is_end = true;
        std::vector<NodePtr> childs;
        bool isLeaf() const {return childs.empty();}
    };
public:
    void insert(std::string_view str);
    //void erase(const std::string& str);
    //bool find(const std::string& str) const;
private:
    NodePtr root_;
};
