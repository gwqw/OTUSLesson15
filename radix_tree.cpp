#include "radix_tree.h"

#include "str_view_utils.h"

using namespace std;

void RadixTree::insert(std::string_view str) {
    if (!root_) {
        root_ = make_unique<Node>(Node{string(str), true});
        return;
    }

    Node* traverseNode = root_.get();
    int symbFound = 0;

    while (traverseNode != nullptr && !traverseNode->isLeaf()
            && symbFound < str.size()) {
        string_view to_find = str.substr(symbFound, str.npos);
        Node* nextNode = nullptr;
        for (const auto& node : traverseNode->childs) {
            if (starts_with(to_find, node->label)) {
                nextNode = node.get();
                break;
            }
        }

        if (nextNode) {

        } else {

        }

    }
}

