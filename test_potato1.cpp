#include "BinarySearchTree.h"
#include <sstream>
#include <cassert>
using namespace std;
// 1 means that there is an empty tree with nullptr, 2 means there is a tree with only a root, 3 means there is a root and a left subtree, 4 means there is a root and a right subtree, 5 means there is a root and a left and right subtree.
void testInsert1();
void testInsert2();
void testInsert3();
void testInsert4();
void testInsert5();
void testEmpty();
void testHeight1();
void testHeight2();
void testHeight3();
void testHeight4();
void testHeight5();
void testMax1();
void testMax2();
void testMax3();
void testMax4();
void testMax5();
void testMin1();
void testMin2();
void testMin3();
void testMin4();
void testMin5();

int main() {
    testInsert1();
    testInsert2();
    testInsert3();
    testInsert4();
    testInsert5();
    testEmpty();
    testHeight1();
    testHeight2();
    testHeight3();
    testHeight4();
    testHeight5();
    testMax1();
    testMax2();
    testMax3();
    testMax4();
    testMax5();
    testMin1();
    testMin2();
    testMin3();
    testMin4();
    testMin5();
}
void testInsert1(){
    // start with an empty tree
    BinarySearchTree<int> bst1;
    assert(bst1.empty());

    // create iterator to the element
    auto i =  bst1.insert(0);
    assert(*i==0);
    auto begin= bst1.begin();
    assert(*begin == 0);
    assert(!bst1.empty());

    begin++;
    assert(begin==bst1.end());
    cout<< "testInsert1 passed" << endl;
}
void testInsert2(){
    // start with a root and add a left subtree
    BinarySearchTree<int> bst1;
    bst1.insert(0);
    // create iterator to the element
    auto i =  bst1.insert(-1);
    assert(*i==-1);
    auto begin= bst1.begin();
    assert(*begin == -1);
    begin++;
    assert(*begin==0);
    begin++;
    assert(begin==bst1.end());
    cout<< "testInsert2 passed" << endl;
}
void testInsert3(){
    // start with a root and left subtree
    BinarySearchTree<int> bst1;
    bst1.insert(0);
    // create iterator to the element
    auto i =  bst1.insert(-1);
    assert(*i==-1);
    auto begin= bst1.begin();
    assert(*begin == -1);
    begin++;
    assert(*begin==0);
    begin++;
    assert(begin==bst1.end());
    // adding left subtree
    bst1.insert(-2);
    auto leftSub = bst1.begin();
    assert(*leftSub==-2); // left of left sub tree
    leftSub++;
    assert(*leftSub==-1);
    leftSub++;
    assert(*leftSub==0); // Root
    leftSub++;
    assert(leftSub==bst1.end());



    cout<< "testInsert3 passed" << endl;
}
void testInsert4(){
    // start with a Node and add right subtree
    BinarySearchTree<int> bst1;
    assert(bst1.empty());
    
    // create iterator to the element
    auto i =  bst1.insert(0);
    assert(*i==0);
    auto begin= bst1.begin();
    assert(*begin == 0);
    assert(!bst1.empty());
    // add right subtree
    auto rightsub =  bst1.insert(1);
    assert(*rightsub==1);
    begin++;
    assert(begin==rightsub);
    cout<< "testInsert4 passed" << endl;
}
void testInsert5(){
    // start with a Node and add right subtree and left subtree
    BinarySearchTree<int> bst1;
    assert(bst1.empty());
    auto root = bst1.insert(0); //root
    assert(*root==0);
    auto left_sub = bst1.insert(-1); //leftsubtree
    assert(*left_sub==-1);
    auto right_sub = bst1.insert(1); //rightsubtree
    assert(*right_sub==1);
    assert(++root==right_sub);
    auto left_left = bst1.insert(-2); //left of left_sub
    assert(*left_left==-2);
    BinarySearchTree<int> :: Iterator begin = bst1.begin();
    assert(*begin==-2);
    begin++; // left sub
    begin++; // root
    begin++; // right sub
    begin++; // end
    assert(begin==bst1.end());

    cout<< "testInsert5 passed" << endl;
}

void testEmpty(){
    //empty
    BinarySearchTree<int> bst1;
    assert (bst1.empty());
    // not empty
    bst1.insert(0);
    assert (!bst1.empty());
    cout << "testEmpty passed"<< endl;
}
void testHeight1(){
    // start with an empty tree
    BinarySearchTree<int> bst1;
    assert(bst1.height()==0);
    cout<< "testHeight1 passed" << endl;
}
void testHeight2(){
    // start with a tree with root
    BinarySearchTree<int> bst1;
    bst1.insert(0);
    assert(bst1.height()==1);
    cout<< "testHeight2 passed" << endl;
}
void testHeight3(){
    // start with a tree with root and left subtree
    BinarySearchTree<int> bst1;
    bst1.insert(0);
    assert(bst1.height()==1);
    bst1.insert(-1);
    assert(bst1.height()==2);
    cout<< "testHeight3 passed" << endl;
}
void testHeight4(){
    // start with a tree with root and right subtree
    BinarySearchTree<int> bst1;
    bst1.insert(0);
    assert(bst1.height()==1);
    // add right sub tree
    bst1.insert(1);
    assert(bst1.height()==2);
    cout<< "testHeight4 passed" << endl;
}
void testHeight5(){
    // start with a tree with root and right and left subtree
    BinarySearchTree<int> bst1;
    bst1.insert(0);
    assert(bst1.height()==1);
    // add right sub tree
    bst1.insert(1);
    //add left subtree
    bst1.insert(-1);

    assert(bst1.height()==2);
    cout<< "testHeight5 passed" << endl;
}
void testMax1(){
    // start with an empty tree
    BinarySearchTree<int> bst1;
    assert(bst1.max_element()==bst1.end());
    cout<< "testMax1 passed" << endl;
}
void testMax2(){
    // start with a tree with root
    BinarySearchTree<int> bst1;
    bst1.insert(0);
    assert(*bst1.max_element()==0);
    cout<< "testMax2 passed" << endl;
}
void testMax3(){
    // start with a tree with root and left subtree
    BinarySearchTree<int> bst1;
    bst1.insert(0);
    assert(bst1.height()==1);
    bst1.insert(-1);
    assert(*bst1.max_element()==0);
    cout<< "testMax3 passed" << endl;
}
void testMax4(){
    // start with a tree with root and right subtree
    BinarySearchTree<int> bst1;
    bst1.insert(0);
    assert(bst1.height()==1);
    // add right sub tree
    bst1.insert(1);
    assert(*bst1.max_element()==1);
    cout<< "testMax4 passed" << endl;
}
void testMax5(){
    // start with a tree with root and right and left subtree
    BinarySearchTree<int> bst1;
    bst1.insert(0);
    assert(bst1.height()==1);
    // add right sub tree
    bst1.insert(1);
    //add left subtree
    bst1.insert(-1);
    assert(*bst1.max_element()==1);

    assert(bst1.height()==2);
    cout<< "testMax5 passed" << endl;
}
void testMin1(){
    // start with an empty tree
    BinarySearchTree<int> bst1;
    assert(bst1.min_element()==bst1.end());
    cout<< "testMin1 passed" << endl;
}
void testMin2(){
    // start with a tree with root
    BinarySearchTree<int> bst1;
    bst1.insert(0);
    assert(*bst1.min_element()==0);
    cout<< "testMin2 passed" << endl;
}
void testMin3(){
    // start with a tree with root and left subtree
    BinarySearchTree<int> bst1;
    bst1.insert(0);
    assert(bst1.height()==1);
    bst1.insert(-1);
    assert(*bst1.min_element()==-1);
    cout<< "testMin3 passed" << endl;
}
void testMin4(){
    // start with a tree with root and right subtree
    BinarySearchTree<int> bst1;
    bst1.insert(0);
    assert(bst1.height()==1);
    // add right sub tree
    bst1.insert(1);
    assert(*bst1.min_element()==0);
    cout<< "testMin4 passed" << endl;
}
void testMin5(){
    // start with a tree with root and right and left subtree
    BinarySearchTree<int> bst1;
    bst1.insert(0);
    assert(bst1.height()==1);
    // add right sub tree
    bst1.insert(1);
    //add left subtree
    bst1.insert(-1);
    assert(*bst1.min_element()==-1);
    
    assert(bst1.height()==2);
    cout<< "testMin5 passed" << endl;
}