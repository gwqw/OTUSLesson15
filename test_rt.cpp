#define BOOST_TEST_MODULE radix_tree_test_module
#include <boost/test/unit_test.hpp>

#include <string_view>

#include "str_view_utils.h"
#include "radix_tree.h"
#include "print_rt.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(str_utils_test_suite)

    BOOST_AUTO_TEST_CASE(test_get_common_prefix) {
        BOOST_TEST(get_common_prefix("test", "test") == "test");
        BOOST_TEST(get_common_prefix("test1", "test") == "test");
        BOOST_TEST(get_common_prefix("test", "test1") == "test");
        BOOST_TEST(get_common_prefix("test1", "test2") == "test");
        BOOST_TEST(get_common_prefix("test", "non_test") == "");
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(radix_tree_test_suite)

    BOOST_AUTO_TEST_CASE(test_insert_example) {
        {
            RadixTree tr;
            tr.insert("aleksey");
            BOOST_TEST(getTreeStructure(tr, simple_test_formater) == "aleksey$\n");
        }
        {
            RadixTree tr;
            tr.insert("aleksey");
            tr.insert("sasha");
            BOOST_TEST(getTreeStructure(tr, simple_test_formater) == "\naleksey$\nsasha$\n");
        }
        {
            RadixTree tr;
            tr.insert("aleksey");
            tr.insert("sasha");
            tr.insert("aleks");
            BOOST_TEST(getTreeStructure(tr, simple_test_formater) == "\nsasha$\naleks$\ney$\n");
        }
        {
            RadixTree tr;
            tr.insert("aleksey");
            tr.insert("sasha");
            tr.insert("aleks");
            tr.insert("alek");
            BOOST_TEST(getTreeStructure(tr, simple_test_formater) == "\nsasha$\nalek$\ns$\ney$\n");
        }
        {
            RadixTree tr;
            tr.insert("aleksey");
            tr.insert("sasha");
            tr.insert("aleks");
            tr.insert("alek");
            tr.insert("alesha");
            BOOST_TEST(getTreeStructure(tr, simple_test_formater) == "\nsasha$\nale\nk$\ns$\ney$\nsha$\n");
        }
        {
            RadixTree tr;
            tr.insert("aleksey");
            tr.insert("sasha");
            tr.insert("aleks");
            tr.insert("alek");
            tr.insert("alesha");
            tr.insert("maksim");
            BOOST_TEST(getTreeStructure(tr, simple_test_formater) == "\nale\nk$\ns$\ney$\nsha$\nsasha$\nmaksim$\n");
        }
    }

    BOOST_AUTO_TEST_CASE(test_insert_to_root) {
        {
            RadixTree tr;
            tr.insert("root");
            tr.insert("test");
            BOOST_TEST(getTreeStructure(tr, simple_test_formater) == "\nroot$\ntest$\n");
        }
        {
            RadixTree tr;
            tr.insert("root");
            tr.insert("root");
            BOOST_TEST(getTreeStructure(tr, simple_test_formater) == "root$\n");
        }
        {
            RadixTree tr;
            tr.insert("root");
            tr.insert("roo");
            BOOST_TEST(getTreeStructure(tr, simple_test_formater) == "roo$\nt$\n");
        }
        {
            RadixTree tr;
            tr.insert("root");
            tr.insert("rooter");
            BOOST_TEST(getTreeStructure(tr, simple_test_formater) == "root$\ner$\n");
        }
        {
            RadixTree tr;
            tr.insert("root");
            tr.insert("room");
            BOOST_TEST(getTreeStructure(tr, simple_test_formater) == "roo\nt$\nm$\n");
        }
    }

    BOOST_AUTO_TEST_CASE(test_insert_to_node) {
        {
            RadixTree tr;
            tr.insert("r");
            tr.insert("root");
            tr.insert("rest");
            BOOST_TEST(getTreeStructure(tr, simple_test_formater) == "r$\noot$\nest$\n");
        }
        {
            RadixTree tr;
            tr.insert("r");
            tr.insert("root");
            tr.insert("root");
            BOOST_TEST(getTreeStructure(tr, simple_test_formater) == "r$\noot$\n");
        }
        {
            RadixTree tr;
            tr.insert("r");
            tr.insert("root");
            tr.insert("roo");
            BOOST_TEST(getTreeStructure(tr, simple_test_formater) == "r$\noo$\nt$\n");
        }
        {
            RadixTree tr;
            tr.insert("r");
            tr.insert("root");
            tr.insert("rooter");
            BOOST_TEST(getTreeStructure(tr, simple_test_formater) == "r$\noot$\ner$\n");
        }
        {
            RadixTree tr;
            tr.insert("r");
            tr.insert("root");
            tr.insert("room");
            BOOST_TEST(getTreeStructure(tr, simple_test_formater) == "r$\noo\nt$\nm$\n");
        }
        {
            RadixTree tr;
            tr.insert("r");
            tr.insert("root");
            tr.insert("room");
            tr.insert("roo");
            BOOST_TEST(getTreeStructure(tr, simple_test_formater) == "r$\noo$\nt$\nm$\n");
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
            BOOST_TEST(tr.find("alek") == 3);
            BOOST_TEST(tr.find("aleks") == 4);
            BOOST_TEST(tr.find("aleksey") == 5);
            BOOST_TEST(tr.find("alesha") == 3);
            BOOST_TEST(tr.find("maksim") == 0);
            BOOST_TEST(tr.find("sasha") == 0);
        }
    }

BOOST_AUTO_TEST_SUITE_END()
