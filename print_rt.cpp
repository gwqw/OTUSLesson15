#include "print_rt.h"

using namespace std;

std::string simple_test_formater(const RadixTree::TreeValue& value, const std::vector<bool>& is_cont) {
    string res = string(value.label);
    if (value.is_end) {
        res += '$';
    }
    return res;
}

std::string simple_formater(const RadixTree::TreeValue& value, const std::vector<bool>& is_cont) {
    string res;
    for (size_t i = 0; i < value.lvl; ++i) {
        res += "  ";
    }
    res += string(value.label);
    if (value.is_end) {
        res += '$';
    }
    return res;
}

std::string pretty_formater(const RadixTree::TreeValue& value, const std::vector<bool>& is_cont) {
    string res;
    for (size_t i = 0; i < value.lvl; ++i) {
        if (is_cont[i]) {
            res += "| ";
        } else {
            res += "  ";
        }
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
    vector<bool> is_continued;
    for (const auto& c : v) {
        if (c.lvl >= is_continued.size()) {
            is_continued.resize(c.lvl+1);
        }
        is_continued[c.lvl] = !c.is_last;
        res += fm(c, is_continued) + '\n';
    }
    return res;
}

std::set<std::string> getTreeStructureForTest(const RadixTree& tr) {
    auto v = tr.getAllValues();
    set<string> res;
    vector<bool> is_continued;
    for (const auto& c : v) {
        res.insert(simple_formater(c, is_continued));
    }
    return res;
}
