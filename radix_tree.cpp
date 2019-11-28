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

        str.remove_prefix(prefix.size());

//        if (prefix != root_->label) {
//            if (prefix.empty()) {
//                update_root("", move(root_->label));
//            } else {
//                update_root(string(prefix), get_suffix(root_->label, prefix));
//            }
//            if (!str.empty()) {
//                add_node(root_.get(), Node{string(str), true});
//            } else {
//                root_->is_end = true;
//            }
//            return;
//        } else if (str.empty()) {
//            root_->is_end = true;
//            return;
//        }


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
        Node* nextNode = nullptr;
        for (auto it =  rootNode->childs.begin(); it != rootNode->childs.end(); ++it) {
            auto& node = *it;
            string_view prefix = get_common_prefix(str, node->label);
            if (prefix == node->label) {
                str.remove_prefix(prefix.size());
                nextNode = node.get();
                break;
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
                old_node->label = get_suffix(old_node->label, prefix);;
                rootNode = add_node(rootNode, Node{string(prefix), false});
                add_node(rootNode, move(old_node));
                str.remove_prefix(prefix.size());
                add_node(rootNode, Node{string(str), true});
                return;
            }
        }
        if (nextNode) {
            rootNode = nextNode;
        } else {
            break;
        }
    }
    if (!str.empty()) {
        add_node(rootNode, Node{string(str), true});
    } else {
        rootNode->is_end = true;
    }
}

void RadixTree::update_root(std::string label, std::string child_label) {
    NodePtr old_root = move(root_);
    old_root->label = move(child_label);
    root_ =  make_unique<Node>(Node{move(label), false});
    root_->childs.push_back(move(old_root));
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
        res.emplace_back(TreeValue{node->label, lvl, node->is_end});
        ++lvl;
        for (auto it = node->childs.rbegin(); it != node->childs.rend(); ++it) {
            st.emplace(it->get(), lvl);
        }
    }
    return res;
}

std::string getTreeStructure(const RadixTree& tr) {
    auto v = tr.getAllValues();
    string res;
    for (auto c : v) {
        res += string(c.label);
        if (c.is_end) {
            res += '$';
        }
        res += '\n';
    }
    return res;
}

