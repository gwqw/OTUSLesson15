#include "radix_tree.h"

#include <utility>
#include <stack>
#include <cassert>

#include "str_view_utils.h"

using namespace std;

void RadixTree::insert(std::string_view str) {
    insert_recursive(root_, str);
}

void RadixTree::insert_recursive(NodePtr& rootNode, std::string_view str) {
    if (!rootNode) {
        rootNode = make_unique<Node>(Node{string(str), true});
        return;
    }

    string_view prefix = get_common_prefix(str, rootNode->label_);
    str.remove_prefix(prefix.size());
    if (prefix == rootNode->label_) {
        if (str.empty()) {
            rootNode->is_end_ = true;
        } else {
            insert_recursive(rootNode->childs[str[0]], str);
        }
    } else {
        // rootNode => new_node(prefix) -> old_node(label - prefix, is_str_empty?)
        //                              -> rest_node(str - prefix, true) (if str isn't empty)
        auto old_node = move(rootNode);
        old_node->label_ = get_suffix(old_node->label_, prefix);
        rootNode = make_unique<Node>(Node{string(prefix), str.empty()});
        rootNode->add_node(move(old_node));
        if (!str.empty()) {
            rootNode->add_node(str[0], Node{string(str), true});
        }
    }
}

std::size_t RadixTree::find(std::string_view str) const {
    // special case for str in root
    if (root_ && !str.empty() && str == root_->label_) return str.size()-1;
    return find_recursive(root_, str);
}

std::size_t RadixTree::find_recursive(const RadixTree::NodePtr &rootNode, std::string_view str) {
    if (!rootNode || str.empty()) { // mustn't go here if str is in tree
        return 0;
    }
    string_view prefix = get_common_prefix(str, rootNode->label_);
    str.remove_prefix(prefix.size());
    if (str.empty()) return 0;

    return prefix.size() + find_recursive(rootNode->childs[str[0]], str);
}

void RadixTree::Node::add_node(RadixTree::NodePtr &&new_node) {
    assert(!new_node->label_.empty());
    childs.emplace(new_node->label_[0], move(new_node));
}

std::vector<RadixTree::TreeValue> RadixTree::getAllValues() const {
    vector<TreeValue> res;
    stack<tuple<Node*, size_t, bool>> st;
    st.emplace(root_.get(), 0, true);
    while (!st.empty()) {
        auto [node, lvl, is_last] = st.top();
        st.pop();
        res.emplace_back(TreeValue{node->label_, lvl, node->is_end_, is_last});
        ++lvl;
        is_last = true;
        for (auto it = node->childs.begin(); it != node->childs.end(); ++it) {
            st.emplace(it->second.get(), lvl, is_last);
            if (it == node->childs.begin()) {
                is_last = false;
            }
        }
    }
    return res;
}

