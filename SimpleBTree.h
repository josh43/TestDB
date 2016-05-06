//
// Created by neo on 5/5/16.
//

#ifndef TESTDATABASE_SIMPLEBTREE_H
#define TESTDATABASE_SIMPLEBTREE_H

#include <algorithm>
#include <exception>
struct SimpleNode{
    SimpleNode(){
        data[0] = -1;
        data[1] = -1;
        children[0] = nullptr;
        children[1] = nullptr;
        children[2] = nullptr;
        numChildrenLeft =3;
        isLeaf = 0;
        parent = nullptr;
    }
    bool isLeaf;
    int data[2];
    int numChildrenLeft;
    SimpleNode * children[3];
    SimpleNode * parent;

};
struct LeafNode : public SimpleNode{
    LeafNode * next;

};
class SimpleBTree{
private:
    int leafLevel;
    SimpleNode * root;
public:

    SimpleBTree(){
        root = new SimpleNode();
        root->isLeaf = 1;
        root->parent == nullptr;
        leafLevel = 0;
    }
    ~SimpleBTree(){
        delete root;
    }
    void insert(int x){
        if(root->numChildrenLeft > 1 && root->isLeaf){
            // since thier are 3 internal nodes
            // this means that thier is space in t he children
            insertInOrder(root,x);
        }else{
            SimpleNode * nodeLoc = findNodeToInsert(x);
            if(nodeLoc->numChildrenLeft >1){
                insertInOrder(nodeLoc,x);
            }else{
                if(nodeLoc == root){
                    // then
                    root->isLeaf = 0;
                    // create two new buckets
                    SimpleNode * bucketOne = new SimpleNode();
                    bucketOne->isLeaf =1;
                    SimpleNode * bucketTwo = new SimpleNode();
                    bucketTwo->isLeaf =1;
                    // insert [0- to n/2) b1
                    int splitIndex = 3/2; // == 1
                    int splitVal = getSplitVal(root->data,x);
                    insertInBucket(bucketOne,0,1,
                                   root,0);
                    // second bucket always has split val
                    insertInBucket(bucketTwo,0,1,
                                   root,splitIndex);
                    insertInOrder(bucketTwo,x);

                    root->data[0] = splitVal;
                    root->data[1] = -1;

                    root->children[0] = bucketOne;
                    root->children[0]->parent = root;
                    root->children[1] = bucketTwo;
                    root->children[1]->parent = root;
                    root->numChildrenLeft++;
                    // [n/2, n) b2

                }else{
                    // else it was not the root and needs additional splitting
                    // if the parent has size just insert into thier
                    if(nodeLoc->parent->numChildrenLeft >1){
                        // will split the nodes into two buckets nodeLoc and rightBucket
                        SimpleNode * rightBucket = new SimpleNode();
                        rightBucket->isLeaf = true;
                        // start
                        // insert [0- to n/2) b1
                        int splitIndex = 3/2; // == 1
                        int splitVal = getSplitVal(nodeLoc->data,x);
                        insertInBucket(nodeLoc,0,1,
                                       nodeLoc,0);

                        // becauser insertInBucket reduces count
                        nodeLoc->numChildrenLeft += (1-0);
                        // second bucket always has split val
                        insertInBucket(rightBucket,0,1,
                                       nodeLoc,splitIndex);
                        // This will make sure that the rest of the children and nulled out
                        // and they have that many less children
                        nodeLoc->data[1] = -1;
                        nodeLoc->numChildrenLeft++;

                        insertInOrder(rightBucket,x);

                        insertInOrder(nodeLoc->parent,splitVal);
                        nodeLoc->parent->numChildrenLeft--;
                        nodeLoc->parent->children[2] = rightBucket;
                        rightBucket->parent = nodeLoc->parent;


                        // where good
                        // only need to create 1 additional bucket

                    }else{
                        // rinse and repeat esay
                    }
                }
                // split node
            }
        }

    }
    static void printTree(SimpleBTree * curr){
        printTree(curr->root);
    }

        static void printTree(SimpleNode * curr){
       if(curr == nullptr){
           return;
       }

                printf("%d %d  numChildrenLeft : %d isLeaf = %s \n",curr->data[0],curr->data[1],curr->numChildrenLeft,curr->isLeaf == 1 ? "YES" : "NO");


           for(int i = 0; i < 3; i++){
               printTree(curr->children[i]);
           }

    }
private:
    SimpleNode * getRightBucket(SimpleNode * toSplit, int x){
        SimpleNode * toReturn = new SimpleNode();
        toReturn->isLeaf = 1;
        insertInBucket(toReturn,0,1,toSplit,1);
        insertInOrder(toReturn,x);
        for(int i = 1;i < 2; i++){
            // for later use obvi this only executes one time
            toSplit->data[i] = -1;
            toSplit->numChildrenLeft++;
        }
        return toReturn;
    }
    int getSplitVal(int * data,int additional){
        int split = 3/2;
        if(data[split-1] < additional && data[split] > additional){
            return additional;
        }
        if(data[split] < additional){
            return data[split];
        }else{
            return data[split -1];
        }
    }
    void insertInBucket(SimpleNode * insertLoc, int start, int end,
                        SimpleNode * util, int utilStart){
        for(int i = start; i < end; i++){
            insertLoc->data[i] = util->data[utilStart++];
            insertLoc->numChildrenLeft--;
            //node->
        }
    }

    SimpleNode * findNodeToInsert(int x){
        SimpleNode * toReturn = root;
        // while its not a leaf
        while(!toReturn->isLeaf) {
            int i;
            for ( i = 0; i < 2; i++) {
                if(x < toReturn->data[i] || toReturn->data[i] == -1){
                    toReturn = toReturn->children[i];
                    break;
                }
            }
            if(i >= 2){
             toReturn = toReturn->children[i];
            }
        }

        return toReturn;
    }
    void insertInOrder(SimpleNode * insertLoc, int x){
        for(int i= 0; i < 2; i++){
            if(insertLoc->data[i] == -1){
                insertLoc->data[i] = x;
                break;
            }
            if(x < insertLoc->data[i]){
                // found the location
                int tmp = insertLoc->data[i];
                insertLoc->data[i] = x;
                int j = 0;
                for( j = i+1; j+1 < 2; j++){
                    insertLoc->data[j] = tmp;
                    tmp = insertLoc->data[j+1];
                }
                // final  insert
                insertLoc->data[j] = tmp;
               // tmp = insertLoc->data[j+1];
                break;
            }
        }


        insertLoc->numChildrenLeft--;
    }
};
#endif //TESTDATABASE_SIMPLEBTREE_H
