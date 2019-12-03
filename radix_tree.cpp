#include "radix_tree.h"

#include <utility>
#include <stack>
#include <cassert>

#include "str_view_utils.h"

using namespace std;

void RadixTree::insert(std::string_view str) {
    // create root
    if (!root_) {
        root_ = make_unique<Node>(Node{string(str), true});
        return;
    }

    // check root and nodes recursive
    string_view prefix = get_common_prefix(str, root_->label_);
    str.remove_prefix(prefix.size());
    if (prefix == root_->label_) {
        if (str.empty()) {
            root_->is_end_ = true;
        } else {
            insert_recursive(root_.get(), str[0], str);
        }
    } else {
        if (prefix.empty()) {
            update_root("", move(root_->label_));
        } else {
            update_root(string(prefix), string(get_suffix(root_->label_, prefix)));
        }
        if (!str.empty()) {
            root_->add_node(str[0], Node{string(str), true});
        } else {
            root_->is_end_ = true;
        }
    }
}

void RadixTree::insert_recursive(Node* rootNode, char letter, std::string_view str) {
    auto it = rootNode->find_node(letter);
    if (!rootNode->is_node_exist(it)) {
        rootNode->add_node(letter, Node{string(str), true});
        return;
    }
    auto& node = it->second;
    string_view prefix = get_common_prefix(str, node->label_);
    str.remove_prefix(prefix.size());

    if (prefix == node->label_) {
        if (str.empty()) {
            node->is_end_ = true;
        } else {
            insert_recursive(node.get(), str[0], str);
        }
    } else {
        // rootNode -> new_node(prefix) -> node(label - prefix, is_str_empty?)
        //                              -> rest_node(str - prefix, true) (if str isn't empty)
        assert(!prefix.empty());
        rootNode = rootNode->insert_node(prefix[0], prefix,
                                         get_suffix(node->label_, prefix), str.empty());
        if (!str.empty()) {
            rootNode->add_node(str[0], Node{string(str), true});
        }
    }
}

std::size_t RadixTree::find(std::string_view str) const {
    if (!root_) return 0;

    size_t cur_pos = 0;
    Node* rootNode = root_.get();
    while (!str.empty()) {
        auto it = rootNode->find_node(str[0]);
        if (!rootNode->is_node_exist(it)) break;
        const auto& node = it->second;
        string_view prefix = get_common_prefix(str, node->label_);

        if (!prefix.empty() && prefix == node->label_) {
            str.remove_prefix(prefix.size());
            if (str.empty() ) {
                return cur_pos;
            }
            rootNode = node.get();
            cur_pos += prefix.size();
        }
    }
    return 0;
}

void RadixTree::update_root(std::string label, std::string child_label) {
    NodePtr old_root = move(root_);
    old_root->label_ = move(child_label);
    root_ =  make_unique<Node>(Node{move(label), false});
    root_->add_node(move(old_root));
}

RadixTree::Node* RadixTree::Node::insert_node(char letter, std::string_view new_label,
        std::string_view old_label, bool is_end) {
    auto new_node = make_unique<Node>(Node{string(new_label), is_end});
    auto old_node = move(childs[letter]);
    old_node->label_ = old_label;
    new_node->add_node(move(old_node));
    auto [it, _] = childs.insert_or_assign(letter, move(new_node));
    return it->second.get();
}

RadixTree::Node* RadixTree::Node::add_node(RadixTree::NodePtr &&new_node) {
    assert(!new_node->label_.empty());
    auto [it, res] = childs.emplace(new_node->label_[0], move(new_node));
    if (res) {
        return it->second.get();
    } else {
        return nullptr;
    }
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

