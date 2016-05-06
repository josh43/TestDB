//
// Created by neo on 5/4/16.
//

#ifndef TESTDATABASE_BTREE_H
#define TESTDATABASE_BTREE_H

#include <cstdint>
#include <printf.h>
#include <stdexcept>

/*
 * B+ tre of order d : each (internal) ndoe has between d and 2 * d entires
 *  except possibly the root
 *  root can have fewer than d keys
 */

template <typename T = int, int numNodes =5, T NULLVALUE = std::numeric_limits<int>::min()>
class BNode{
    friend class BTree< T ,numNodes , NULLVALUE >;
private:
    // if nodes == nullptr than you have reached a leaf
    BNode<T,numNodes> * nodes;
    T compare[numNodes +1];
    int8_t numFree;
public:
    BNode(){
        nodes = new BNode<T,numNodes>[numNodes];
        for(int i = 0; i < numNodes; i ++){
            for(int j =0; j < numNodes; j++){
                nodes[i].compare[j] = NULLVALUE;

            }
        }
        numFree = numNodes+1;
    }
    ~BNode(){


        delete [] nodes;
    }
};
template <typename T = int, int numNodes =5, T NULLVALUE = std::numeric_limits<int>::min()>
class BTree{
private:
    typedef const T * Elements;
    Elements  elements;
    unsigned int leafLevel;
    BNode<T,numNodes> * head;
    T & locationLookup(const T & element,int level,BNode<T,numNodes> * curr){
        if(level > leafLevel+1){
            return nullptr;
        }
        for(int i = 0; i < numNodes; i++){
            if(curr->compare[i] == NULLVALUE && level == leafLevel){
                return curr->compare[i];
            }else if( element < curr->compare[i]){

            }else if( element == curr->compare[i]){
                return curr->compare[i];
            }
            // else it must be greater
        }
    }

    void easyInsert(BNode<T, numNodes, NULLVALUE> *pNode, T &&toInsert){
        if(pNode == nullptr){
            printf("Error pNode was nullptr on line 66 in BTree.h\n");
            throw std::invalid_argument("!!");
        }
        elements = pNode->compare;
        if(pNode->numFree == numNodes){
            pNode[0] = toInsert;
            return;
        }
        pNode->numFree--;
        // else insert
        for(int i =0; i < numNodes; i++){
            if(elements[i] == NULLVALUE){
                elements[i] = toInsert;
                return;
            }
            if(toInsert < elements[i]){
                // found it
                // bubble everything over
                //3 4 20 NULL NULL
                //insert 2
                //
                //3 4 NULL 20
                int j =  numNodes;
               while(elements[j-1] != NULLVALUE) {
                   j--;
               }


                for(; j >= 1  ; j--){
                    if(toInsert > elements[j-1]){
                        elements[j] = toInsert;
                        return;
                    }
                    std::swap(elements[j],elements[j-1]);

                }
            }
        }

        printf("ERROR with easy insert you should never reach here!!\n");
    }
    void insertHelper(T && element, uint8_t level, BNode<T,numNodes,NULLVALUE> * curr){
        if(level == leafLevel){
            // yay insert if thier is space
            if(curr->numFree >0){
                easyInsert(curr,element);
            }else{
                printf("YOU DIDNT HANDLE THIS CASE FOR INSERTING WHEN FULL!\n");
            }
        }
    }
public:
    BTree(){
        leafLevel = 1;
        head = new BNode<T,numNodes,NULLVALUE>();
    }
    const T & lookup(const T & element){

    }
    void insert(const T  &element){
        insert(T(element));
        //copy the elment

    }
    void insert(T && element){
        int8_t i = 0;
        int8_t currLevel = 0;
        for(;i < numNodes;i++){
            if(head[i] == NULLVALUE){
                break; // insert hear
            }
            if(element <head[i]){
                break;
            }
        }
        // take the element
    }
    ~BTree(){
        if(head)
            delete head;
    }


};




#endif //TESTDATABASE_BTREE_H
