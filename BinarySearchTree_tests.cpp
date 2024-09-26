// Project UID db1f506d06d84ab787baf250c265e24e

#include "BinarySearchTree.h"
#include "unit_test_framework.h"
#include <sstream>

using namespace std;
TEST(test_basic) {
    BinarySearchTree<int> tree;
    
    ASSERT_TRUE(tree.empty());
    tree.insert(8);
    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(tree.size() == 1);
    ASSERT_TRUE(tree.height() == 1);
    tree.insert(2);
    ASSERT_TRUE(tree.size() == 2);
    ASSERT_TRUE(tree.height() == 2);
    tree.insert(1);
    tree.insert(11);
    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(tree.size() == 4);
    ASSERT_TRUE(tree.height() == 3);
    ASSERT_TRUE(*tree.min_element() == 1);
    ASSERT_TRUE(*tree.max_element() == 11);
    ASSERT_TRUE(tree.check_sorting_invariant());
    ASSERT_TRUE(*tree.min_greater_than(8) == 11);
    ASSERT_FALSE(*tree.min_greater_than(2) == 1);
}

TEST(test_basic1) {
    BinarySearchTree<int> tree;

    tree.insert(9);
    tree.insert(3);
    tree.insert(1);
    tree.insert(4);
    tree.insert(8);
    tree.insert(6);
    tree.insert(7);

    ASSERT_TRUE(tree.size() == 7);
    ASSERT_TRUE(tree.height() == 6);
    tree.insert(5);
    ASSERT_TRUE(tree.height() == 6);
    ASSERT_TRUE(tree.check_sorting_invariant());
}

TEST(test_find) {
    BinarySearchTree<int> tree;

    tree.insert(4);
    tree.insert(9);
    tree.insert(5);
    tree.insert(2);
    tree.insert(10);
    tree.insert(11);
    tree.insert(12);

    BinarySearchTree<int>::Iterator Find_it;
    Find_it = tree.find(6);
    ASSERT_TRUE(Find_it == tree.end());
    ASSERT_TRUE(tree.check_sorting_invariant());

    Find_it = tree.find(2);
    *Find_it = 3;
    ASSERT_TRUE(tree.check_sorting_invariant());

    Find_it = tree.find(9);
    *Find_it = 3;
    ASSERT_FALSE(tree.check_sorting_invariant());
}

TEST(test_min_max) {
    BinarySearchTree<int> tree;

    tree.insert(7);
    tree.insert(3);
    tree.insert(4);
    tree.insert(10);
    tree.insert(2);
    tree.insert(9);
    tree.insert(8);
    tree.insert(11);


    ASSERT_TRUE(tree.size() == 8);
    ASSERT_TRUE(tree.height() == 4);
    ASSERT_TRUE(*tree.min_element() == 2);
    ASSERT_TRUE(*tree.max_element() == 11);

    BinarySearchTree<int> newtree;    
    ASSERT_TRUE(newtree.min_element() == newtree.end());
    ASSERT_TRUE(newtree.max_element() == newtree.end());
}

TEST(test_traverse) {
    BinarySearchTree<int> tree;
    tree.insert(7);
    tree.insert(3);
    tree.insert(6);
    tree.insert(10);
    tree.insert(4);
    tree.insert(8);
    tree.insert(2);
    tree.insert(5);
    tree.insert(11);
    std::ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    ASSERT_TRUE(oss_preorder.str() == "7 3 2 6 4 5 10 8 11 ");

    std::ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    ASSERT_TRUE(oss_inorder.str() == "2 3 4 5 6 7 8 10 11 ");

    BinarySearchTree<int> newtree;
    newtree.insert(1);
    newtree.insert(2);
    newtree.insert(3);
    newtree.insert(4);
    newtree.insert(5);

    std::ostringstream oss_preorder_2;
    newtree.traverse_preorder(oss_preorder_2);
    ASSERT_TRUE(oss_preorder_2.str() == "1 2 3 4 5 ");

    std::ostringstream oss_inorder_2;
    newtree.traverse_inorder(oss_inorder_2);
    ASSERT_TRUE(oss_inorder_2.str() == "1 2 3 4 5 ");

    BinarySearchTree<int> new2tree;
    new2tree.insert(5);
    new2tree.insert(4);
    new2tree.insert(3);
    new2tree.insert(2);
    new2tree.insert(1);

    std::ostringstream oss_preorder_3;
    new2tree.traverse_preorder(oss_preorder_3);
    ASSERT_TRUE(oss_preorder_3.str() == "5 4 3 2 1 ");

    std::ostringstream oss_inorder_3;
    new2tree.traverse_inorder(oss_inorder_3);
    ASSERT_TRUE(oss_inorder_3.str() == "1 2 3 4 5 ");
}

TEST(test_min_greater) {
    BinarySearchTree<int> tree;
    tree.insert(6);
    tree.insert(2);
    tree.insert(9);
    tree.insert(3);
    tree.insert(7);
    tree.insert(8);
    tree.insert(1);

    ASSERT_TRUE(*tree.min_greater_than(0) == 1);
    ASSERT_TRUE(*tree.min_greater_than(1) == 2);
    ASSERT_TRUE(*tree.min_greater_than(2) == 3);
    ASSERT_TRUE(*tree.min_greater_than(3) == 6);
    ASSERT_TRUE(*tree.min_greater_than(4) == 6);
    ASSERT_TRUE(*tree.min_greater_than(5) == 6);
    ASSERT_TRUE(*tree.min_greater_than(6) == 7);
    ASSERT_TRUE(*tree.min_greater_than(7) == 8);
    ASSERT_TRUE(*tree.min_greater_than(8) == 9);
    ASSERT_TRUE(tree.min_greater_than(9) == tree.end());
}

TEST(test_copy) {
    BinarySearchTree<int> tree;
    tree.insert(6);
    tree.insert(2);
    tree.insert(5);
    tree.insert(1);
    tree.insert(8);

    BinarySearchTree<int> tree_copy = tree;
    *tree.find(8) = 9;
    ASSERT_TRUE(*tree.max_element() == 9);
    ASSERT_TRUE(*tree_copy.max_element() == 8);

    *tree.find(2) = 3;
    *tree.find(1) = 2;
    ASSERT_TRUE(*tree.min_element() == 2);
    ASSERT_TRUE(*tree_copy.min_element() == 1);

    ASSERT_TRUE(*tree.min_greater_than(2) = 3);
    ASSERT_TRUE(*tree_copy.min_greater_than(2) = 5);

    *tree.begin() = 12;
    ASSERT_FALSE(tree.check_sorting_invariant());
    ASSERT_TRUE(tree_copy.check_sorting_invariant());
}
TEST_MAIN()
