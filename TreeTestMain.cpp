#include "AvlTree.h"
#include <string>
#include <memory>

using namespace std;

int compare(int a , int b){
    if(a>b)
        return -1;
    if(a<b)
        return 1;
    return 0;
}

int compare2(shared_ptr<int> a , shared_ptr<int> b){
    if(a.get()>b.get())
        return -1;
    if(a.get()<b.get())
        return 1;
    return 0;
}
int main(){
    /*
    auto* tree = new AvlTree<int>;
    int c =-1 ,b =-1;
    tree->add(1,compare);
    tree->add(2,compare);
    tree->add(3,compare);
    tree->add(4,compare);
    //tree->add(5,compare);
    //tree->add(6,compare);
    tree->print();
    tree->add(5,compare,c,b);
    printf("left: %d, right: %d\n",c, b);
    tree->print();

    tree->add(5,compare);
    tree->add(6,compare,c ,b);
    printf("left: %d, right: %d\n",c, b);

    tree->add(10,compare);
    tree->add(11,compare);
    tree->add(12,compare);
    tree->add(13,compare);
    tree->add(7,compare);
    tree->print();

    tree->add(8,compare,c ,b);
    printf("inserted:%d left: %d, right: %d\n",8,c, b);
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

    int array1[12] = { 12,13,14,15,16,17,18,19,20,31 ,32,33};
    bool a = tree->createTreeFromSortedArray(array1,12);
    tree->print();
    int *max = tree->findMaxLowerThanX(25 , compare);
    cout <<max[0] <<endl;
    int *min = tree->findMinBiggerThanX(25, compare);
    cout <<min[0] <<endl;
    tree->print();
    int max2 = tree->findMax();
    cout <<max2 <<endl;
    int min2 = tree->findMin();
    cout <<min2 <<endl;
    tree->remove(17,compare);
    tree->print();
    cout << a <<endl;

    delete tree;
    */

    return 0;
}