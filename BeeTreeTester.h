//
// Created by neo on 5/7/16.
//

#ifndef TESTDATABASE_BEETREETESTER_H
#define TESTDATABASE_BEETREETESTER_H

#include "BeeTree.h"
typedef BeeTree<int,5> Tree;
/*
 * Test case todos
 * 1. merge two levels both from right most nodes
 * 2. merge two levels both from middle nodes
 * 3. merge from middle node to a merge to left node
 * 4. merge from middle node to a merge to right node
 * 5. the other two combos^^
 * 6. Insert a bunch of data, delete it see what happens
 */
static void removeFromRoot(){
    Tree theTree;
    assert(theTree.remove(3) == false);
    theTree.insert(3);
    theTree.remove(3);
    assert(theTree.getSize() == 0);
    theTree.insert(5);
    theTree.insert(3);
    assert(theTree.getSize() == 2);
    theTree.remove(5);
    assert(theTree.find(5) == false);
    assert(theTree.find(3) == true);

}
static void testMergeLevelOneFromLeft(Tree root){
    root.remove(5);
    root.remove(8);


    // should remerge
}
static void testMergeLevelOneFromRight(Tree root){
    root.remove(20);
    root.remove(25);

    root.insert(30);
    root.insert(35);
    root.insert(40);

    root.remove(30);
    root.remove(45);

}

static void testMergeTwoLevels(Tree root){
    root.insert(3);
    root.insert(4);
    // 2 data
    root.insert(1);
    root.insert(2);
    // 3 data
    root.insert(-1);
    root.insert(-2);
    // 4 data
    root.insert(-5);
    root.insert(-6);

    root.remove(-5);
    root.remove(-6);


}
static void testMergeLevelOneFromMiddle(Tree root){
    root.insert(30);
    root.insert(35);
    root.insert(40);

    root.remove(20);
    root.remove(25);
    root.remove(15);
    //mid left rmove
    root.insert(20);
    root.insert(25);
    //mid right rmove
    root.remove(35);
    root.remove(40);
}
static void testRedisFromRight(Tree root){



}
static void testRedisFromLeft(Tree root){

    root.insert(45);

    root.remove(15);
    root.remove(8);
    assert(root.find(15) == false);
    assert(root.find(8) == false);

}
static void testBtree(){
    BeeTree<int,5> b;
    b.insert(5);
    b.insert(8);
    b.insert(20);
    b.insert(25);
    b.insert(15);


    //removeFromRoot();
    //testRedisFromRight(b);
    //testRedisFromLeft(b);
   // testMergeLevelOneFromLeft(b);
    //testMergeLevelOneFromRight(b);
    testMergeTwoLevels(b);
    testMergeLevelOneFromMiddle(b);
}
#endif //TESTDATABASE_BEETREETESTER_H
