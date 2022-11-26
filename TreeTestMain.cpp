#include "AvlTree.h"
#include <string>
#include <iostream>
using namespace std;

int main(){
    AvlTree<int>* tree = new AvlTree<int>;
    tree->add(1);
    tree->add(2);
    tree->add(3);
    tree->add(4);
    tree->add(5);
    tree->add(5);
    tree->add(6);
    tree->add(7);
    tree->find(1);
    tree->add(8);
    tree->add(9);
    tree->add(10);
    tree->add(11);
    tree->add(12);
    tree->add(13);
    tree->print();
    tree->remove(4);
    tree->remove(3);
    tree->print();
    tree->remove(2);
    tree->remove(10);
    tree->print();
    tree->remove(5);
    tree->print();
    tree->remove(8);
    tree->print();
    tree->remove(20);
    tree->print();

    return 0;
}