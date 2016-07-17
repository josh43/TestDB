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

const int MERGE_LEFT = -1;
const int MERGE_RIGHT = -2;
/*
 * TODO
 * 1. Put a delete mechanism
 * 2. Fix the SPLIT_INDEX so that you can have trees of children 2 or 3
 * 3. Do a rebalancing algo for zee tree
 */
template <typename T>
// num children really stands for the number of data elements confusing riight
// however if you have 3 elements you are assumed to have 3+ 1 children
// unless you are the root

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
        // i think this is off by one.. :(
        return numChildren > MAX_DATA;
    }
    bool underflow(){
        // under flow for 3 children is 1
        // if nodes == 7 chen you can have 4-7
        //
        return numChildren < ceil(MAX_CHILDREN/2);
    }
    bool vergeOfUnderFlow(){
        // for 3 children
        // MAX_CHILDREN/2 ciel is 2
        return numChildren == ceil(MAX_CHILDREN/2);
    }
    T * getMax(){
        return data[numChildren-1];
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
    unsigned int size;
    NodeType * root;
    int leafLevel;
    const int  MAX_DATA;
    const int  SPLIT_INDEX;
public:
    typedef   Iterator<NodeType>  IterType;
    // I split like [0,MAX_CHILDREN/2 -1] and [MAX_CHILDREN/2,MAX_CHILDREN]
    BeeTree():MAX_DATA(MAX_CHILDREN-1),SPLIT_INDEX((MAX_CHILDREN-1)/2),size(0){
        // I want the number of data to be at least 4
        // Why? Because their are way too many edge cases for 2-3
        // Consider deleting from a leaf where that node is the last key
        // if you try and bubble up the tree and remove the value
        // you might accidentally try and assign it to the node that has just been deleted
        // bubbling is my thing it only taks O(h) height of the tree and is gewd
        assert(MAX_DATA >= 4);
        leafLevel = 0;
        root = new NodeType();

    }
    void insert(const T & element){
        T toInsert = element;
        insert(std::move(toInsert));
    }
    void insert(T && element){
        T * theElement = new T(element);
       // printf("Inserting %d\n",element);
        NodeType * insertIndex = getInsertLocation(*theElement);
        insertWithinNode(insertIndex,theElement);
       // BeeTree<T,MAX_CHILDREN>::printTree(this);
        size++;
        insertIndex->numChildren++;
        adjust(insertIndex);
       // BeeTree<T,MAX_CHILDREN>::printTree(this);


    }
    const unsigned int & getSize(){
        return this->size;
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
        NodeType * removeIndex = getInsertLocation(element);
        if(isInNode(element,removeIndex) >=0){
            // remove it
            removeFromNodeBubble(element,removeIndex->parent);
            removeFromNode(element,removeIndex);
            adjust(removeIndex);
            size--;
            return true;
        }

        return false;

    }
    bool find(const T & element){
        NodeType * node  = getInsertLocation(element);
        return (isInNode(element,node) >=0);
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

    void printLinkedList(){
        Iterator<Node<T,MAX_CHILDREN> > * itr = this->getIterator();
        const Node<int,4> * dat;
        printf("Linked List begin\n");
        while(!itr->isEmpty()){
            dat = itr->getData();
            for(int i = 0; i < 4; i++){
                if(dat->data[i]){
                   printf("%d ",*(dat->data[i]));
                }
            }
            itr->next();
        }
        printf("\n");
    }
private:
    NodeType * getInsertLocation(const T & element){
        // This potentially wont find it
        // Consider the case where all the elements are the same number
        // except for one element

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
                }else if(element <= *(toReturn->data[i])){
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
    /*void bubbleRemove(const T & element,  NodeType * node){
        // this will not handle the merges/rebalances
       if(node->parent == nullptr){
           // root just remove it
           removeFromNode(element,node);
           return;
       }else{
           if(isInNode(element,node->parent)){
               // remove from parent as well
               removeFromNode(element,node);
               bubbleRemove(element,node->parent);
           }else{
               // just remove it and
               removeFromNode(element,node);
           }
           // you are going to have to
       }

    }
     */
    void removeFromNodeBubble(const T & element, NodeType * node){
        // This function is basically just going to bubble up and try and replace
        // element if it can
        // originally passed in the parent
        if(node == nullptr){
            return;
        }
        int location;
        if((location =isInNode(element,node)) >= 0){
            int nextBiggest = node->children[location]->numChildren-2;
            T * toInsert = node->children[location]->data[nextBiggest];

            // its on the left so we want the max of the current
            // for the case when the node is empty in a tree with 3 children
            // the node will actually have no more elements because having 1
            // is still fine ciel(3/2) -1  == 1 i s ok :(
            // I think their are too many of these dumb edge cases
            // which will slow the tree down
            if(toInsert != nullptr) {
                // we found a sub
                // dont' delete it just replace it,
                // parents arent responsible for mem
                node->data[location] = nullptr;
                insertWithinNode(node, new T(*toInsert));
                removeFromNodeBubble(element,node->parent);

            }

            // remove from parent by replacing it with one of its children

        }
    }
    void removeFromNode(const T & element, NodeType *  node){
        for(int i =0 ; i < MAX_DATA; i ++){
            if(*(node->data[i]) == element){
                delete node->data[i];
                for(int j = i; j +1 < MAX_DATA; j++){
                    node->data[j] = node->data[j+1];
                }
                // set the last node
                node->numChildren--;
                node->data[MAX_DATA-1] = nullptr;
                return;
            }
        }

        throw std::invalid_argument("Couldn't find element in the node you passed\n");
    }
    int isInNode(const T & element, const NodeType * node){
        if(node == nullptr){
            return -1;
        }
        for(int i = 0; i < MAX_DATA;i++){
            if(!node->data[i]){
                return -1;
            }else if((*(node->data[i])) == element){
                return i;
            }
        }
        return -1;
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
        }else if(node->underflow() && (node->parent != nullptr)){
            // if it is not the root
                    // If you CANNOT redistribute than you need to merge!
            // fun so kewl so fresh
            // remember after merging check for underflow!
                if (!(redistribute(node))) {
                    int loc = getLocationOfNodeFromParent(node);
                    NodeType * merger = nullptr;
                    if(loc !=node->parent->numChildren){
                        // merge node and node's right sibling
                        merger = merge(node,node->parent->children[loc+1]);


                            // else it was the root
                            // set the overwritten node to nullptr brahhskee
                        merger->parent->children[loc+1] = nullptr;
                            // basically just overite left parent val and bubb
                            // need to cover the case where it is the right most
                        // remember to set the new max
                        merger->parent->data[loc] = merger->getMax();
                        bubbleLeft(merger->parent,loc+1);
                        merger->parent->numChildren--;
                        if(merger->parent == root && root->numChildren == 0){
                            // MERGER COMPETE!
                            root = merger;
                            root->parent = nullptr;
                            this->leafLevel--;
                        }
                        adjust(merger->parent);

                    }else{
                        // merge left;
                        // this is the special case when we merged to the rightmost
                        // node we dont need to bubble

                        merger = merge(node,node->parent->children[loc-1]);
                        // set to the min

                        // when removing from the right most node you dont have to worry
                        // about taking from the min it can be the max!
                        merger->parent->children[loc] = nullptr;
                        node->parent->children[loc-1] = merger;
                        node->parent->data[loc - 1] = merger->getMax();
                        merger->parent->numChildren--;
                        if(merger->parent == root && root->numChildren == 0){
                            // MERGER COMPETE!
                            root = merger;
                            root->parent = nullptr;

                            this->leafLevel--;
                        }
                        adjust(node->parent);


                        // merge node nad node's left sibling
                    }

                }

        }
    }

    void bubbleLeft(NodeType * curr,int nullNodeLoc){
       // it should never be the case that curr->children[0] is null
        // all indices are inclusive
        int i =0;
        for(i = nullNodeLoc; i < MAX_DATA; i--){
            if(!curr->data[i+1]){
                // reached the end
                break;
            }
           curr->data[i] = curr->data[i+1];
            curr->children[i] = curr->children[i+1];
        }

        // there is one extra child ALWAYS Amiga
        curr->children[i] = nullptr;

    }
    NodeType * merge(NodeType * curr, NodeType * mergeThis){

        // two cases when mergering from left and right
        if(curr->data[0] > mergeThis->data[0]){
            // just switch the nodes if you are trying to do a right merge
            NodeType * tmp = mergeThis;
            mergeThis = curr;
            curr = tmp;
        }
        int offset = curr->numChildren;
        for( int i = 0; i < mergeThis->numChildren; i ++){
            curr->data[offset+i] = mergeThis->data[i];
            curr->children[offset + i] = mergeThis->children[i];
            curr->numChildren++;
            if(mergeThis->children[i]) {
                mergeThis->children[i]->parent = curr;
            }
        }

        // don't forget to set the link
        curr->children[MAX_CHILDREN-1] = mergeThis->children[MAX_CHILDREN-1];
        return curr;
    }

    bool redistribute(NodeType * node){
        if(node->parent == nullptr){
            // shouldnt call when node->parent is nullptr
            // the root can have min amount of nodes
            return true;
        }else{
            int location = getLocationOfNodeFromParent(node);
            if(location == 0){
                // check right
                return redisHelper(node,location,nullptr,node->parent->children[1]);
            }else if(location == MAX_CHILDREN){
                //check left
                return redisHelper(node,location,node->parent->children[MAX_CHILDREN-1],nullptr);

            }else{
                return redisHelper(node,location,node->parent->children[location-1],node->parent->children[location+1]);
                // check left and right
            }
        }
    }
    bool redisHelper(NodeType * underflow, int location,NodeType * n1, NodeType * n2){
        // check n1 first
        if(n1){
            if(!(n1->vergeOfUnderFlow())){
                // n1 is on my left
                // so take max data from left
                T * cpy = new T(*(n1->data[n1->numChildren-1]));
                insertWithinNode(underflow,cpy);
                removeFromNode(*(n1->data[n1->numChildren-1]),n1);
                underflow->parent->data[location-1] = n1->getMax();
                underflow->numChildren++;
                return true;
            }
        }
        // check n2
        if(n2){
            if(!(n2->vergeOfUnderFlow())){
                // n2 is on my right
                T * cpy = new T(*(n2->data[0]));
                insertWithinNode(underflow,cpy);
                removeFromNode(*(n2->data[0]),n2);
                underflow->parent->data[location] = cpy;
                underflow->numChildren++;

                return true;
            }
        }

        return false;
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
            NodeType * tmp = toSplit->children[MAX_CHILDREN];
            toSplit->children[MAX_CHILDREN] = right;
            // basically swapping the pointers!! OCOOOOOOL BRAH
            if(tmp){
                toSplit->children[MAX_CHILDREN] = tmp;
            }
        }

        toReturn.first = toSplit;
        toReturn.second = right;
        return toReturn;
    }

    // bubbles them al lright
    void bubbleChildren(NodeType * parent, NodeType * child,int startingFromHere){
        NodeType * toAssign = child;
        NodeType * nextLink = nullptr;
        if(isLeaf(toAssign)){
            // remember to make that lin

                nextLink = parent->children[startingFromHere];
                 if(nextLink == nullptr){
                     // this happens when we are inserting at the end
                     // in which case we have to get the last next pointer and attach
                     // it to child
                     if(startingFromHere>0 && child != parent->children[startingFromHere - 1]->children[MAX_CHILDREN]) {
                         // make sure its not linked
                         nextLink = parent->children[startingFromHere - 1]->children[MAX_CHILDREN];
                     }
                // try and set it to the previous
                 }
            if(startingFromHere > 0){
                parent->children[startingFromHere-1]->children[MAX_CHILDREN] = child;
            }
        }

        for(int i= startingFromHere; i < MAX_CHILDREN+1; i++){

            if(parent->children[i] == nullptr){
                parent->children[i] = new NodeType();
                parent->children[i] = toAssign;
                toAssign->parent = parent;
                parent->numChildren++;
                break;
            }else {
                NodeType *tmp = parent->children[i];
                parent->children[i] = toAssign;
                toAssign->parent = parent;
                toAssign = tmp;
            }
        }
        // set the links
        if(nextLink) {

            child->children[MAX_CHILDREN] = nextLink;
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
        //printf("Memory is good");
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
