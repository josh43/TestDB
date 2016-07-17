//
// Created by neo on 5/5/16.
//

#ifndef TESTDATABASE_BEETREE_H
#define TESTDATABASE_BEETREE_H

#include <cmath>
#include <stdexcept>
#include <assert.h>
#include <stack>
#include <queue>

/*
 * TODO
 * 1. Put a delete mechanism
 * 2. Fix the SPLIT_INDEX so that you can have trees of children 2 or 3
 * 3. Do a rebalancing algo for zee tree
 */
template <typename T>
class Iterator{
public:
    virtual  Iterator<T> * getIterator(){return nullptr;};
    virtual Iterator<T>  * next(){return nullptr;};
    const T * getData() {
        return currIterator;
    }
   bool isEmpty(){
       return currIterator == nullptr;
   }
    Iterator(){}
protected:
    T * currIterator;
};
// Got some psuedo from http://www.cs.uofs.edu/~mccloske/courses/cmps340/btree_alg.pdf
FILE * theLog= NULL;
template <typename T, int MAX_CHILDREN = 3>
class Node{
public:
    static const int MAX_DATA = MAX_CHILDREN-1;
    typedef Node<T,MAX_CHILDREN> NodeType;
    //overflows ar
    T * data[MAX_DATA+1];
    NodeType *children[MAX_CHILDREN+1];
    NodeType * parent;

    int numChildren;
    bool overflow(){
        return numChildren > MAX_DATA;
    }
    bool underflow(){
        return numChildren < ceil(MAX_CHILDREN/2)-1;
    }

    Node(){
        numChildren = 0;
        parent = nullptr;
       for(int i =0; i < MAX_DATA +1; i++){
           data[i] = nullptr;
       }
        for(int i =0; i < MAX_CHILDREN+1; i++){
            children[i] = nullptr;

        }

    }

    ~Node(){
        // dont implement
    }
public:

};
template <typename T, int MAX_CHILDREN = 3>
class BeeTree : public Iterator<Node<T,MAX_CHILDREN> > {
private:
    typedef Node<T,MAX_CHILDREN> NodeType;
    NodeType * root;
    int leafLevel;
    const int  MAX_DATA;
    const int  SPLIT_INDEX;
public:
    typedef   Iterator<NodeType>  IterType;
    // I split like [0,MAX_CHILDREN/2 -1] and [MAX_CHILDREN/2,MAX_CHILDREN]
    BeeTree():MAX_DATA(MAX_CHILDREN-1),SPLIT_INDEX(MAX_CHILDREN/2){
        assert(MAX_DATA >= 2);
        leafLevel = 0;
        root = new NodeType();

    }
    void insert(const T & element){
        T toInsert = element;
        insert(std::move(toInsert));
    }
    void insert(T && element){
        T * theElement = new T(element);
        printf("Inserting %d\n",element);
        NodeType * insertIndex = getInsertLocation(theElement);
        insertWithinNode(insertIndex,theElement);
       // BeeTree<T,MAX_CHILDREN>::printTree(this);
        insertIndex->numChildren++;
        adjust(insertIndex);
       // BeeTree<T,MAX_CHILDREN>::printTree(this);


    }
    bool remove(const T & element){
        // case 1
        // x is a leaf ndoe and has > MAX_CHILDREN/2 KEYS
        // case 2 The node x is a leaf and x has exactly MAX_CHILDREN/2 -1 keys
        // ie the min number of keys that it should have so it will underflow upon
        // removing
        // case 2.a
        /*          If x has a sibling with at least MAX_CHILDREN/2 keys then move
         *          x's parent key into x and move the appropriate extreme of
         *
         */
        const NodeType * insertIndex = getInsertLocation(element);
        if(isInNode(element,insertIndex)){
            // remove it
            bubbleRemove(element,insertIndex);
            adjust(element);

            return true;
        }

        return false;

    }
    static void printTree(BeeTree * curr){
        printf("PRINTING TREE \n\n");
        printParrallel(curr->root);
        /*
        int i;
        for(i = 0; i < MAX_CHILDREN-1; i++){
            printBeeHelper(curr->root->children[i],6,0,MAX_CHILDREN-1);
            // then print
            if(curr->root->data[i]) {
                prettyPrint(*(curr->root->data[i]), 3);
            }
        }
        printBeeHelper(curr->root->children[i],3,0,MAX_CHILDREN-1);
        */
        /*
        printBeeHelper(curr->root->children[0],4,0,MAX_CHILDREN-1);
        // then print
        prettyPrint(*(curr->root->data[0]),2);
        printBeeHelper(curr->root,2,1,MAX_CHILDREN-1);
         */
        if(theLog){
            fprintf(theLog,"\nThe height is %d\n",curr->leafLevel);

        }

        printf("END PRINTING TREE \n\n");

    }

    virtual  Iterator<Node<T,MAX_CHILDREN> > *getIterator() override {
        this->currIterator = root;
        while(this->currIterator->children[0]){
            this->currIterator = this->currIterator->children[0];
        }
        return this;
    }

    virtual Iterator<Node<T,MAX_CHILDREN> > *next() override {
        if(this->currIterator){
            this->currIterator = this->currIterator->children[MAX_CHILDREN];
            return this;
        }else{
            return nullptr;
        }

    }


private:
    NodeType * getInsertLocation(const T * element){
        /*
         * This is just a simple sequential search
         * You can make it binary
         * but it goes through and while the element is greater than an index we advance
         * until either the element is less than a data or the data is null
         * then we have found it-
         */
        NodeType * toReturn = root;

        // while its not a leaf
        int currLevel = 0;
        while(currLevel < leafLevel) {
            int i;
            for ( i = 0; i < MAX_DATA; i++) {
                if(toReturn->data[i] == nullptr){

                        toReturn = toReturn->children[i];
                        break;
                }else if(*element <= *(toReturn->data[i])){
                    // IMPOSE <= because of how I am splitting!!!!
                    //
                    // I would put this in the same if statement as above
                    // but I am not positive that the left statement
                    // will always be checked first
                    // but I am not alwa
                    toReturn = toReturn->children[i];
                    break;
                }
            }
            if(i >= MAX_DATA){
                toReturn = toReturn->children[i];
            }
            currLevel++;
        }

        return toReturn;
    }
    void bubbleRemove(const T & element, const NodeType * node){
        if(node == nullptr){
            return;
        }
        int index;
        int indexTwo;
        if((index = isInNode(element,node))){
            if((indexTwo = isInNode(element,node->parent))){
                //
                bubbleRemove(node->parent,element);

            }
            // remove after
            // remove at index and
        }

    }
    int isInNode(const T & element, const NodeType * node){
        if(node == nullptr){
            return false;
        }
        for(int i = 0; i < MAX_CHILDREN;i++){
            if(!node->children[i]){
                return 0;
            }else if(*(node->children[i]) == element){
                return i;
            }
        }
        return false;
    }
    void insertWithinNode(NodeType * node, T * x){

        for(int i= 0; i < MAX_DATA+1; i++){
            if(node->data[i] == nullptr){
                node->data[i] = x;
                break;
            }
            if(*x > *(node->data[i])) {
                continue;
                // this next statement doesnt do anything
                // but i am debugging so im leaving it
                // aka im trying diff things
            } else if(*x <= *(node->data[i])){
                // found the location
                // bubble everything over
                T  * tmp = node->data[i];
                node->data[i] = x;
                int j = 0;
                for( j = i+1; j+1 < MAX_DATA+1; j++){
                    T * swap = node->data[j];
                    node->data[j] = tmp;
                    tmp = swap;
                }
                // final  insert
                if(node->data[j] == nullptr){
                    node->data[j] = new T();
                }
                node->data[j] = tmp;
                break;
            }
        }
        //BeeTree<T,MAX_CHILDREN>::printTree(this);


    }

    /*
     * Very important
     * The parent nodes of all leaves will only ever have week pointers
     *  So if you free memory be sure to set parents back
     */
    void adjust(NodeType * node){
        if(node->overflow()){

            std::pair<NodeType *, NodeType *> split = getSplit(node);
            if(split.first->parent == nullptr){
                // you split the root
                // new root
                // you split ZEE ROOT
                // This isnt returning nulld values
                // do it ahead of time for search
               // BeeTree<T,MAX_CHILDREN>::printTree(this);

                leafLevel++;
                NodeType * coolRoot = new Node<T,MAX_CHILDREN>();
                checkNodeContents(coolRoot);
                coolRoot->children[0] = split.first;
                coolRoot->children[1] = split.second;
                //I think i am still suffering from too many nodes on one level
                split.first->parent = coolRoot;
                split.second->parent = coolRoot;
                // the smallest of the larger split
                coolRoot->data[0] = split.first->data[SPLIT_INDEX];
                coolRoot->numChildren++;

                root = coolRoot;
          //      BeeTree<T,MAX_CHILDREN>::printTree(this);
//
                // delete root;
                //insert(*toInsert);
                //delete toInsert;
                //
            }else{

                // insert into one of the parent nodes and check adjusting
                // Note that it will never be the case that the parent is overflowing
                // before bubbling. If the current node was full
                // and its parent was full, it still has one more slot
                // so upon bubbling it will then overflow which i check
                int leftLoc = getLocationOfNodeFromParent(split.first);
               // BeeTree<T,MAX_CHILDREN>::printTree(this);
                bubbleChildren(node->parent,split.second,leftLoc+1);
               // BeeTree<T,MAX_CHILDREN>::printTree(this);

                // smallest of right tree
                // remember that split.first->data[SPLIT_INDEX] is already in tree
                // you need to insert sec


                insertWithinNode(node->parent,new T(*(split.first->data[SPLIT_INDEX])));
               // BeeTree<T,MAX_CHILDREN>::printTree(this);

                if(node->parent->overflow()) {
                    adjust(node->parent);
                }
            }
        }else if(node->underflow()){

        }
    }

    int getLocationOfNodeFromParent(const NodeType * node){
        if(node == nullptr || node->parent == nullptr){
            throw std::invalid_argument("Either the node or its parent was nullptr in gettingLocationOfNodeFromParent:(\n");

        }
        for(int i = 0; i < MAX_CHILDREN;i++){
            if(node == node->parent->children[i]){
                return i;
            }
        }
        throw std::invalid_argument("For some reason you forgot to handle the case where when inserting into a nodes parent it was overflowing");
    }
    std::pair<NodeType *, NodeType *> getSplit(NodeType * toSplit) {
        typedef std::pair<NodeType *, NodeType *> NodePair;
        NodePair toReturn;
        // this is causing me a ton of errors its fucking returning initalized memory
        // when i explicitly NULL IT OUT
        NodeType *right = nullptr;
        right =new Node<T,MAX_CHILDREN>();
        checkNodeContents(right);

        // remember these indices includeisve < x < exclusive
        // so dont include (SPLIT_INDEX +1) otherwise it will be smaller
        insertInBucket(right, 0, MAX_DATA- (SPLIT_INDEX),
                        toSplit,SPLIT_INDEX+1);
        // remember to insert the right most node in case its thier!

        int i;
        int j;

        // delete children
        // o
        // you only ever set the 2 children

        // MAX_CHILDREN + 1 because i store an extra child for cheats sakes!
        for(i = SPLIT_INDEX+1, j= 0; i < MAX_CHILDREN+1; i ++,j++){
            // zero out and remove
            if(toSplit->children[i] == nullptr){
                break;
            }else {
                right->children[j] = toSplit->children[i];
                right->children[j]->parent = right;
                // dont delete if you are just chaning
                //delete toSplit->children[i];
                toSplit->children[i] = nullptr;
            }
        }

        /* Not sure I need this
         * if(toSplit->data[MAX_DATA] && (toSplit->parent != nullptr)){


            insertWithinNode(right,new T(*(toSplit->data[MAX_DATA])));
        }
         */
        // delete data
        // remember that we allow an additional element
        for(i = SPLIT_INDEX+1; i < MAX_DATA+1; i++){
            if(toSplit->data[i]){
                // I delete it because I insert new copies
                // this should be something i change..
                //delete toSplit->data[i];
                toSplit->numChildren--;

                toSplit->data[i] = nullptr;
            }else{
                break;
            }

        }
        if(isLeaf(right) && isLeaf(toSplit)){
            // karmakarmakarmakamillion
            toSplit->children[MAX_CHILDREN] = right;
        }

        toReturn.first = toSplit;
        toReturn.second = right;
        return toReturn;
    }

    void bubbleChildren(NodeType * parent, NodeType * child,int startingFromHere){
        NodeType * toAssign = child;
        for(int i= startingFromHere; i < MAX_CHILDREN+1; i++){

            if(parent->children[i] == nullptr){
                parent->children[i] = new NodeType();
                parent->children[i] = toAssign;
                toAssign->parent = parent;
                parent->numChildren++;
                return;
            }
            NodeType * tmp = parent->children[i];
            parent->children[i] = toAssign;
            toAssign->parent = parent;
            toAssign = tmp;
        }

    }
    void insertInBucket(NodeType * insertHere,int start, int end,NodeType * from,int offset){
        for(int i = start; i < end; i++,offset++){
            if(insertHere->data[i] == nullptr){
                if(from->data[offset] == nullptr){
                    break;
                }
                insertHere->data[i] = new T(*(from->data[offset]));
            }else{
                insertHere->data[i] = from->data[offset];
            }
        }
        insertHere->numChildren+=(end-start);
    }


    bool isLeaf(NodeType * t){
        return (t->children[0] == nullptr);

    }


        void checkNodeContents(NodeType * t){
        for(int i =0; i < MAX_CHILDREN+1; i ++){
            if(t->children[i] != nullptr){
                throw std::invalid_argument("IT IS INITALIZING MEMORY HATE LIFE\n");
            }
        }

        for(int i =0; i < MAX_DATA+1; i ++){
            if(t->data[i] != nullptr){
                throw std::invalid_argument("IT IS INITALIZING MEMORY HATE LIFE\n");
            }
        }
        printf("Memory is good");
    }

    static void printBeeHelper(NodeType * bNode,int level, int start,int end){
        if(bNode == nullptr){
            return;
        }
        int i;
        printLet('-',level);
        printf("\n");
        for(i= start; i < end ; i++){

                if(bNode->data[i]) {

                    prettyPrint(*(bNode->data[i]),level);
                    printBeeHelper(bNode->children[i], level +level, 0,MAX_CHILDREN-1);
                }else if(bNode->children[i]){
                    printBeeHelper(bNode->children[i], level + level, 0,MAX_CHILDREN-1);

                }
        }
        if(bNode->children[i]) {
            //prettyPrint(*(bNode->data[i]),level);
            printBeeHelper(bNode->children[i], level + level, 0,1);
            // then print next node
        }

    }
    static void prettyPrint(int x, int level){
        printLet(' ', level);
        printf("%d\n",x);
        printLet(' ', level);
        printString("|\n", 1);
        printLet(' ', level);
        printString("|\n", 1);
    }
    static void printParrallel( const NodeType * t){
        if(t == nullptr){
            return;
        }

        unsigned int  emptyNodes =0;
        unsigned int  totalNodes =0;
        std::queue<const NodeType *> q;
        q.emplace(t);
        const NodeType * curr= nullptr;
        unsigned long nextNewLine = 0;
        // when you increment it will go to 0 hopefully
        unsigned long currNodesRead = std::numeric_limits<unsigned long>::max();
        while(!q.empty()){
            currNodesRead++;
            curr = q.front();
            if(curr != nullptr){
                emptyNodes+= MAX_CHILDREN- curr->numChildren;
                totalNodes+=MAX_CHILDREN;

                for(int i= 0; i < (MAX_CHILDREN-1) && (curr->data[i] != nullptr); i ++){
                    if(theLog){
                        fprintf(theLog,"%d ",*(curr->data[i]));

                    }else{
                        printf("%d ",*(curr->data[i]));

                    }
                }
                for(int i= 0; i < (MAX_CHILDREN); i++){
                    q.emplace(curr->children[i]);
                }
            }
            if(currNodesRead == nextNewLine){
                if(nextNewLine == 0){
                    nextNewLine = MAX_CHILDREN;
                }else {
                    nextNewLine *= nextNewLine;
                }
                if(theLog){
                    fprintf(theLog,"\n");
                }else {
                    printf("\n");
                }

            }
            q.pop();

        }
        if(theLog){
            fprintf(theLog,"\n\nThe number of empty nodes is %u  out of %u possible\n",emptyNodes,totalNodes);

        }else{
            printf("The number of empty nodes is %u  out of %u possible\n",emptyNodes,totalNodes);

        }
    }
    static void printLet(char c, int count){
        while(count > 0){
            printf("%c",c);
            if(theLog){
                fprintf(theLog,"%c",c);
            }
            count--;
        }
    }
    static void printString(char * c, int count){
        while(count > 0){
            printf("%s",c);
            fprintf(theLog,"%s",c);

            count--;
        }
    }

};
#endif //TESTDATABASE_BEETREE_H
