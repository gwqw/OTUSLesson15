#include "radix_tree.h"

#include <utility>
#include <stack>

#include "str_view_utils.h"

using namespace std;

void RadixTree::insert(std::string_view str) {
    // create root
    if (!root_) {
        root_ = make_unique<Node>(Node{string(str), true});
        return;
    }

    // check root
    if (!root_->label.empty()) {
        string_view prefix = get_common_prefix(str, root_->label);
        if (prefix == root_->label) {
            str.remove_prefix(prefix.size());
            if (str.empty()) {
                root_->is_end = true;
                return;
            }
            // go further
        } else if (prefix.empty()) {
            update_root("", move(root_->label));
            add_node(root_.get(), Node{string(str), true});
            return;
        } else if (prefix == str) { // prefix != root->label
            update_root(string(prefix), get_suffix(root_->label, prefix));
            root_->is_end = true;
            return;
        } else {
            update_root(string(prefix), get_suffix(root_->label, prefix));
            str.remove_prefix(prefix.size());
            add_node(root_.get(), Node{string(str), true});
            return;
        }
    }

    /* Algo:
     * go forward while we have prefix in node
     * if we have full prefix: cut it from str
     * if we have partial prefix:
     *      add new node with suffix (from str)
     *      add new node with sufix (from node if needed)
     * inv: rootNode is OK (already done)
     */
    Node* rootNode = root_.get();
    while (!rootNode->isLeaf() && !str.empty()) {
        auto [it, prefix] = get_closest_node(rootNode, str);
        if (it == rootNode->childs.end()) break;

        auto& node = *it;
        if (prefix == node->label) {
            str.remove_prefix(prefix.size());
            rootNode = node.get();
        } else if (prefix == str) {
            NodePtr old_node = move(node);
            rootNode->childs.erase(it);
            old_node->label = get_suffix(old_node->label, prefix);
            rootNode = add_node(rootNode, Node{string(prefix), true});
            add_node(rootNode, move(old_node));
            return;
        } else if (!prefix.empty()) {
            NodePtr old_node = move(node);
            rootNode->childs.erase(it);
            old_node->label = get_suffix(old_node->label, prefix);
            rootNode = add_node(rootNode, Node{string(prefix), false});
            add_node(rootNode, move(old_node));
            str.remove_prefix(prefix.size());
            add_node(rootNode, Node{string(str), true});
            return;
        } else { // prefix is empty
            if (!str.empty()) {
                add_node(rootNode, Node{string(str), true});
            } else {
                rootNode->is_end = true;
            }
            return;
        }
    }
    if (!str.empty()) {
        add_node(rootNode, Node{string(str), true});
    } else {
        rootNode->is_end = true;
    }
}

std::size_t RadixTree::find(std::string_view str) const {
    if (!root_) return 0;

    size_t cur_pos = 0;
    Node* rootNode = root_.get();
    while (!rootNode->isLeaf() && !str.empty()) {
        auto [it, prefix] = get_closest_node(rootNode, str);
        if (it == rootNode->childs.end()) break;
        const auto& node = *it;

        if (!prefix.empty() && prefix == node->label) {
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
    old_root->label = move(child_label);
    root_ =  make_unique<Node>(Node{move(label), false});
    add_node(root_.get(), move(old_root));
}

std::pair<RadixTree::NodeIter, std::string_view> RadixTree::get_closest_node(RadixTree::Node *root, std::string_view str) {
    for (auto it =  root->childs.begin(); it != root->childs.end(); ++it) {
        string_view prefix = get_common_prefix(str, (*it)->label);
        if (!prefix.empty()) {
            return {it, prefix};
        }
    }
    return {root->childs.end(), ""};
}

RadixTree::Node *RadixTree::add_node(RadixTree::Node *node, RadixTree::NodePtr &&new_node) {
    node->childs.push_back(move(new_node));
    return node->childs.back().get();
}

std::vector<RadixTree::TreeValue> RadixTree::getAllValues() const {
    vector<TreeValue> res;
    stack<pair<Node*, size_t>> st;
    st.emplace(root_.get(), 0);
    while (!st.empty()) {
        auto [node, lvl] = st.top();
        st.pop();
        res.emplace_back(TreeValue{node->label, lvl, node->is_end, node->isLeaf()});
        ++lvl;
        for (auto it = node->childs.rbegin(); it != node->childs.rend(); ++it) {
            st.emplace(it->get(), lvl);
        }
    }
    return res;
}

std::string simple_formater(const RadixTree::TreeValue& value) {
    string res = string(value.label);
    if (value.is_end) {
        res += '$';
    }
    return res;
}

std::string pretty_formater(const RadixTree::TreeValue& value) {
    string res;
    for (size_t i = 0; i < value.lvl; ++i) {
        res += "| ";
    }
    res += "+ " + string(value.label);
    if (value.is_end) {
        res += '$';
    }
    return res;
}

std::string getTreeStructure(const RadixTree& tr, Formater fm) {
    auto v = tr.getAllValues();
    string res;
    for (const auto& c : v) {
        res += fm(c) + '\n';
    }
    return res;
}
