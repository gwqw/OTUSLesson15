#define BOOST_TEST_MODULE radix_tree_test_module
#include <boost/test/unit_test.hpp>

#include <string_view>
#include <iostream>

#include "str_view_utils.h"
#include "radix_tree.h"
#include "print_rt.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(str_utils_test_suite)

    BOOST_AUTO_TEST_CASE(test_get_common_prefix) {
        BOOST_CHECK(get_common_prefix("test", "test") == "test");
        BOOST_CHECK(get_common_prefix("test1", "test") == "test");
        BOOST_CHECK(get_common_prefix("test", "test1") == "test");
        BOOST_CHECK(get_common_prefix("test1", "test2") == "test");
        BOOST_CHECK(get_common_prefix("test", "non_test") == "");
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(radix_tree_test_suite)

    BOOST_AUTO_TEST_CASE(test_insert_example) {
        {
            RadixTree tr;
            tr.insert("aleksey");
            BOOST_CHECK(getTreeStructure(tr, simple_test_formater) == "aleksey$\n");
        }
        {
            RadixTree tr;
            tr.insert("aleksey");
            tr.insert("sasha");
            set<string> expected{"", "  aleksey$", "  sasha$"};
            BOOST_CHECK(getTreeStructureForTest(tr) == expected);
        }
        {
            RadixTree tr;
            tr.insert("aleksey");
            tr.insert("sasha");
            tr.insert("aleks");
            set<string> expected{"", "  aleks$", "  sasha$", "    ey$"};
            BOOST_CHECK(getTreeStructureForTest(tr) == expected);
        }
        {
            RadixTree tr;
            tr.insert("aleksey");
            tr.insert("sasha");
            tr.insert("aleks");
            tr.insert("alek");
            set<string> expected{"", "  alek$", "    s$", "      ey$", "  sasha$"};
            BOOST_CHECK(getTreeStructureForTest(tr) == expected);
        }
        {
            RadixTree tr;
            tr.insert("aleksey");
            tr.insert("sasha");
            tr.insert("aleks");
            tr.insert("alek");
            tr.insert("alesha");
            set<string> expected{"", "  ale", "    k$", "      s$", "        ey$", "    sha$", "  sasha$"};
            BOOST_CHECK(getTreeStructureForTest(tr) == expected);
        }
        {
            RadixTree tr;
            tr.insert("aleksey");
            tr.insert("sasha");
            tr.insert("aleks");
            tr.insert("alek");
            tr.insert("alesha");
            tr.insert("maksim");
            set<string> expected{"", "  ale", "    k$", "      s$", "        ey$", "    sha$", "  sasha$", "  maksim$"};
            BOOST_CHECK(getTreeStructureForTest(tr) == expected);
        }
    }

    BOOST_AUTO_TEST_CASE(test_insert_to_root) {
        {
            RadixTree tr;
            tr.insert("root");
            tr.insert("test");
            set<string> expected{"", "  root$", "  test$"};
            BOOST_CHECK(getTreeStructureForTest(tr) == expected);
        }
        {
            RadixTree tr;
            tr.insert("root");
            tr.insert("root");
            BOOST_CHECK(getTreeStructure(tr, simple_test_formater) == "root$\n");
        }
        {
            RadixTree tr;
            tr.insert("root");
            tr.insert("roo");
            BOOST_CHECK(getTreeStructure(tr, simple_test_formater) == "roo$\nt$\n");
        }
        {
            RadixTree tr;
            tr.insert("root");
            tr.insert("rooter");
            BOOST_CHECK(getTreeStructure(tr, simple_test_formater) == "root$\ner$\n");
        }
        {
            RadixTree tr;
            tr.insert("root");
            tr.insert("room");
            BOOST_CHECK(getTreeStructure(tr, simple_test_formater) == "roo\nt$\nm$\n");
        }
    }

    BOOST_AUTO_TEST_CASE(test_insert_to_node) {
        {
            RadixTree tr;
            tr.insert("r");
            tr.insert("root");
            tr.insert("rest");
            BOOST_CHECK(getTreeStructure(tr, simple_test_formater) == "r$\noot$\nest$\n");
        }
        {
            RadixTree tr;
            tr.insert("r");
            tr.insert("root");
            tr.insert("root");
            BOOST_CHECK(getTreeStructure(tr, simple_test_formater) == "r$\noot$\n");
        }
        {
            RadixTree tr;
            tr.insert("r");
            tr.insert("root");
            tr.insert("roo");
            BOOST_CHECK(getTreeStructure(tr, simple_test_formater) == "r$\noo$\nt$\n");
        }
        {
            RadixTree tr;
            tr.insert("r");
            tr.insert("root");
            tr.insert("rooter");
            BOOST_CHECK(getTreeStructure(tr, simple_test_formater) == "r$\noot$\ner$\n");
        }
        {
            RadixTree tr;
            tr.insert("r");
            tr.insert("root");
            tr.insert("room");
            BOOST_CHECK(getTreeStructure(tr, simple_test_formater) == "r$\noo\nt$\nm$\n");
        }
        {
            RadixTree tr;
            tr.insert("r");
            tr.insert("root");
            tr.insert("room");
            tr.insert("roo");
            BOOST_CHECK(getTreeStructure(tr, simple_test_formater) == "r$\noo$\nt$\nm$\n");
        }
    }

    BOOST_AUTO_TEST_CASE(test_find) {
        {
            RadixTree tr;
            tr.insert("aleksey");
            tr.insert("sasha");
            tr.insert("aleks");
            tr.insert("alek");
            tr.insert("alesha");
            tr.insert("maksim");
            BOOST_CHECK(tr.find("alek") == 3);
            BOOST_CHECK(tr.find("aleks") == 4);
            BOOST_CHECK(tr.find("aleksey") == 5);
            BOOST_CHECK(tr.find("alesha") == 3);
            BOOST_CHECK(tr.find("maksim") == 0);
            BOOST_CHECK(tr.find("sasha") == 0);
        }
    }

BOOST_AUTO_TEST_SUITE_END()
