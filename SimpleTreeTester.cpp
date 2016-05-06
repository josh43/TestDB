#include "SimpleBTree.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char * argv[]) {


    SimpleBTree btree;
    btree.insert(3);
    btree.insert(2);
    btree.insert(5);
    btree.insert(1);
    btree.insert(12);
    btree.insert(4);
    btree.insert(10);
    SimpleBTree::printTree(&btree);

    return 0;
}