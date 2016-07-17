#include <iostream>
#include <signal.h>
#include <limits>
#include <set>
#include <random>
#include "Time.h"

//#include "SimpleBTree.h"
#include "BeeTree.h"
#include "BeeTreeTester.h"
using namespace std;



int main(int argc, char * argv[]) {


    testBtree();

    /*
    theLog = fopen("TheTreeLog.txt","w");
    if(theLog == NULL){
        exit(0);
    }
     */
    const int numNodes =7;
    BeeTree<int,numNodes> b;

    unsigned long start = __rdtsc();
    unsigned long end = __rdtsc();
    srand(3);
    std::set<int> s;
    std::vector<int> test;


    int testSize = 8000000;
    int progress = 0;
    int incremenet = testSize / 100;
    start_exec_timer();

    printf("Time took to insert into a Vector\n");
    for(int i = 0; i< testSize;i++){

        int val = rand() % numeric_limits<int>::max()-100000;

        if( progress*incremenet + incremenet  <i ){
            printf("Vector %d\n%",progress++);
        }
        test.push_back(val);
        // test.push_back(val);

    }
    print_exec_timer();
    printf("Done inserting into a Vector\n");
    progress = 0;

    /*
    start_exec_timer();

    printf("Time took to insert into a Set\n");
    for(int i = 0; i< testSize;i++){


        if( progress*incremenet + incremenet  <i ){
            printf("set %d%\n",progress++);
        }
        s.insert(test[i]);
        // test.push_back(val);

    }
    printf("Done inserting into a Set\n");

    print_exec_timer();

    int i = 0;
     */
    start_exec_timer();
    progress = 0;
    printf("Time took to insert into yoru tree\n");
    for(int i = 0; i< testSize;i++){


        if( progress*incremenet + incremenet  <i ){
            printf("tree %d\n%",progress++);
        }
        b.insert(test[i]);
        // test.push_back(val);

    }
    printf("Done inserting into a Tree\n");

    printf("Time took to insert into your tree\n");
    print_exec_timer();
    std::vector<int> compare;
    /*
    b.insert(5);
    b.insert(1);
    b.insert(6);
    b.insert(2);
    BeeTree<int,4>::printTree(&b);

    b.insert(11);
    b.insert(13);
    b.insert(15);
    b.insert(17);
    b.insert(23);
*/

    BeeTree<int,numNodes>::IterType * itr = b.getIterator();
    const Node<int,numNodes> * dat;
    while(!itr->isEmpty()){
        dat = itr->getData();
        for(int i = 0; i < 4; i++){
            if(dat->data[i]){
                compare.push_back(*(dat->data[i]));
            }
        }
        itr->next();
    }
    auto setIt = s.begin();
    if(s.size() != compare.size()){
        printf("ERROR BRAH\n");

    }
    for(auto vecIt = compare.begin();vecIt != compare.end() && setIt != s.end() ;vecIt++,setIt++){
        if(*vecIt != *setIt){
            printf("ERROR BRAH\n");
        }
    }
   // BeeTree<int,4>::printTree(&b);
    fclose(theLog);
    theLog = NULL;
    exit(0);
    /*SimpleBTree btree;
    btree.insert(3);
    btree.insert(2);
    btree.insert(5);
    btree.insert(1);
    btree.insert(12);
    btree.insert(4);
    btree.insert(10);
    SimpleBTree::printTree(&btree);
     */

    /*
     *
     */



    return 0;
}