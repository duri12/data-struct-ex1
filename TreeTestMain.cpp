#include "AvlTree.h"
#include <string>
#include <iostream>
using namespace std;

int compare(int a , int b){
    if(a>b)
        return -1;
    if(a<b)
        return 1;
    return 0;
}

int main(){
    AvlTree<int>* tree = new AvlTree<int>;
    tree->add(1,compare);
    tree->add(2,compare);
    tree->add(3,compare);
    tree->add(4,compare);
    tree->add(5,compare);
    tree->add(5,compare);
    tree->add(6,compare);
    tree->add(7,compare);
    tree->find(1,compare);
    tree->add(8,compare);
    tree->add(9,compare);
    tree->add(10,compare);
    tree->add(11,compare);
    tree->add(12,compare);
    tree->add(13,compare);
    tree->print();
    tree->remove(4,compare);
    tree->remove(3,compare);
    tree->print();
    tree->remove(2,compare);
    tree->remove(10,compare);
    tree->print();
    tree->remove(5,compare);
    tree->print();
    tree->remove(8,compare);
    tree->print();
    tree->remove(20,compare);
    tree->print();

    return 0;
}