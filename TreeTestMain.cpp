#include "AvlTree.h"
#include <string>

using namespace std;

int compare(int a , int b){
    if(a>b)
        return -1;
    if(a<b)
        return 1;
    return 0;
}

int main(){
    auto* tree = new AvlTree<int>;
    shared_ptr<int> temp = std::make_shared<int>(0);
    weak_ptr<int> rightptr = temp;
    weak_ptr<int> leftptr = temp;
    tree->add(1,compare);
    tree->add(2,compare);
    tree->add(3,compare);
    tree->add(4,compare);
    tree->add(5,compare);
    tree->add(6,compare);
    tree->print();
    //tree->add(5,compare,leftptr,rightptr);
    printf("left: %d, right: %d\n",*leftptr.lock(),*rightptr.lock());    tree->print();

    tree->add(5,compare);
    //tree->add(6,compare,leftptr ,rightptr);
    printf("left: %d, right: %d\n",*leftptr.lock(),*rightptr.lock());    tree->add(10,compare);
    tree->add(11,compare);
    tree->add(12,compare);
    tree->add(13,compare);
    tree->add(7,compare);
    tree->print();

    //tree->add(8,compare,leftptr ,rightptr);
    printf("left: %d, right: %d\n",*leftptr.lock(),*rightptr.lock());
    tree->print();
    int array[10]= {};
    tree->treeToArrayInOrder(array,10);
    for (int i : array)
        cout << i << endl;
    tree->remove(9,compare);

    tree->remove(3,compare);

    tree->remove(2,compare);

    tree->remove(10,compare);


    tree->remove(5,compare);

    tree->remove(8,compare);

    tree->remove(20,compare);
    tree->print();

    int array1[12] = { 12,13,14,15,16,17,18,19,20,21 ,22,23};
    bool a = tree->createTreeFromSortedArray(array1,12);
    tree->print();
    a= !a ;
    delete tree;

    return 0;
}