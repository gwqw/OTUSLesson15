#pragma once

#include <string>
#include <vector>
#include <set>

#include "radix_tree.h"

using Formater = std::string (*)(const RadixTree::TreeValue&, const std::vector<bool>&);

std::string getTreeStructure(const RadixTree& tr, Formater fm);
std::set<std::string> getTreeStructureForTest(const RadixTree& tr);


std::string simple_test_formater(const RadixTree::TreeValue& value, const std::vector<bool>& is_cont);
std::string simple_formater(const RadixTree::TreeValue& value, const std::vector<bool>& is_cont);
std::string pretty_formater(const RadixTree::TreeValue& value, const std::vector<bool>& is_cont);