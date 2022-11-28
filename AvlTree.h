#ifndef DATA_STRUCT_EX1_AVLTREE_H
#define DATA_STRUCT_EX1_AVLTREE_H
#include <iostream>
using namespace std;


template<typename T>
class Node{
    private:
    T _data;
    int _height{};
    int BF{}; // the balanceFactor of the tree note : _bf is reserved by the language
    Node<T>* _left;
    Node<T>* _right;
    Node<T>* _parent;
    public:

    explicit Node(const T& data) ;
    Node(const T& data, Node<T>* left, Node<T>* right, Node<T> parent) ;
    ~Node();
    const T& getData();
    int getheight();
    int getBF();
    Node<T>* getLeft();
    Node<T>* getRight();
    Node<T>* getParent();
    void setLeft(Node<T>* left);
    void setRight(Node<T>* right);
    void setParent(Node<T>* parent);
    void setData(const T& data);
    void setheight(int height);
    void setBF(int BF);
    void print() const ;

};



template<typename T>
class  AvlTree{

    private:
    Node<T>* _root;


    Node<T>* find(Node<T>* current_node,const T& value ,int (*compare)(T,T));
    Node<T>* insert(const T& value,Node<T>* current_node, int (*compare)(T,T)); // the recursive private function that inserts the node
    Node<T>* insert(const T& value ,Node<T>* current_node ,int (*compare)(T,T),const T& left , const T& right); // the recursive private function that
    Node<T>* remove(Node<T>* current_node,const T& value, int (*compare)(T,T)); // the recursive private function
    Node<T>* balance(Node<T>* current_node); // handles the rotation of the tree
    void update(Node<T>* node); // updates the height of the node and the BFactor
    void printSubtree(Node<T>* root ,const string& prefix);
    const T& findMax(Node<T>* node);
    const T& findMin(Node<T>* node);

    Node<T>* LLR(Node<T>* node); //Left-left rotation of the tree
    Node<T>* LRR(Node<T>* node); //Left-right rotation of
    Node<T>* RLR(Node<T>* node); //Right-left rotation of the tree
    Node<T>* RRR(Node<T>* node); //Right-right rotation of the


    public:
    AvlTree();
    ~AvlTree();
    bool add(const T& value, int (*compare)(T,T));
    bool add(const T& value, int (*compare)(T,T),const T& left , const T& right);
    Node<T>* getRoot();
    void print();//prints the tree (copied from the internet)
    Node<T>* find(const T& value, int (*compare)(T,T)); // calls private find with the root
    int height(Node<T>* node);
    int BalanceFactor(Node<T>* node);
    Node<T>* RotateRight(Node<T>* node);
    Node<T>* RotateLeft(Node<T>* node);
    const T& findMax();
    const T& findMin();
    bool remove(const T& value, int (*compare)(T,T));

};

template<typename T>
AvlTree<T>::AvlTree() {
    this->_root = nullptr;
}

template<typename T>
Node<T> *AvlTree<T>::getRoot() {
    return this->_root;
}

template<typename T>
Node<T> *AvlTree<T>::find(const T &value, int (*compare)(T,T)) {
    if(this->_root == nullptr||compare == nullptr){
        return nullptr;
    }
    Node<T>* result = find(this->_root, value,compare);
    if(compare(result->getData() ,value) !=0){ // if value is not  in the tree return  nullptr instead
        return nullptr;
    }
    return result->getData();
}

template<typename T>
Node<T> *AvlTree<T>::find( Node<T> *current_node,const T &value, int (*compare)(T,T)) { // returns the closest node
    if(current_node == nullptr||compare == nullptr){
        return nullptr;
    }
    else if (compare(current_node->getData() , value) == -1 && current_node->getLeft() != nullptr){ // current_node->getData() > value && current_node->getLeft() != nullptr
        return find(current_node->getLeft() , value,compare);
    }
    else if (compare(current_node->getData() , value) == 1 && current_node->getRight() != nullptr){//current_node->getData() < value && current_node->getRight() != nullptr
        return find(current_node->getRight() , value,compare);
    }
    return current_node;
}

template<typename T>
int AvlTree<T>::height(Node<T> *node) {
    if(node == nullptr)
        return 0;
    return node->getHeight();
}

template<typename T>
int AvlTree<T>::BalanceFactor(Node<T> *node) {
    if(node == nullptr)
        return 0;
    return node->getLeft()->getheight() - node->getRight()->getheight();
}



template<typename T>
bool AvlTree<T>::add(const T &value,int (*compare)(T,T)) {
    if(compare==nullptr){
        return false;
    }
    if (this->_root == nullptr){
        try{
            this->_root = new Node<T>(value);

        }
        catch(std::bad_alloc&){
            return false;
        }
        return true;
    }
    Node<T>* temp = this->find(value ,compare);
    if(temp != nullptr){
        return false; // already added
    }
    this->_root = insert(value, this->_root , compare);
    return true;

}
template<typename T>
bool AvlTree<T>::add(const T &value, int (*compare)(T, T), const T &left, const T &right) {
    if(compare==nullptr){
        return false;
    }
    if (this->_root == nullptr){
        try{
            this->_root = new Node<T>(value);

        }
        catch(std::bad_alloc&){
            return false;
        }
        left = nullptr;
        right = nullptr;
        return true;
    }
    Node<T>* temp = this->find(value ,compare);
    if(temp != nullptr){
        return false; // already added
    }
    this->_root = insert(value, this->_root , compare,left,right);
    return true;
}

template<typename T>
Node<T> *AvlTree<T>::insert(const T &value, Node<T> *current_node, int (*compare)(T,T)) {
    if(compare == nullptr){
        return  nullptr;
    }
    if (current_node == nullptr){
        Node<T>* new_node = nullptr;
        try {
            new_node = new Node<T>(value);
        }
        catch (const std::bad_alloc&) {
            delete new_node;
            return nullptr;
        }
        return new_node;
    }
    if(compare(current_node->getData() , value)==-1){//current_node->getData() > value
        current_node->setLeft(insert(value, current_node->getLeft() ,compare));
    }
    else{
        current_node->setRight(insert(value, current_node->getRight(),compare));
    }
    update(current_node);
    return  balance(current_node);
}

template<typename T>
void AvlTree<T>::update(Node<T> *node) {
    int lh =-1 ;
    int rh =-1 ;
    if(node == nullptr)
        return;
    if(node->getLeft()!= nullptr){
        lh = node->getLeft()->getheight();
    }
    if(node->getRight()!= nullptr){
        rh = node->getRight()->getheight();
    }
    node->setheight(1+std::max(rh,lh));
    node->setBF(rh-lh);
}

template<typename T>
Node<T> *AvlTree<T>::balance(Node<T> *current_node) {
    if(current_node->getBF() == -2){ // left heavy tree node
        if(current_node->getLeft()->getBF()<= 0){
            return LLR(current_node);
        }
        else{
            return LRR(current_node);
        }
    }
    else if(current_node->getBF() == 2){ // right heavy tree node
        if(current_node->getRight()->getBF()>= 0){
            return RRR(current_node);
        }
        else{
            return RLR(current_node);
        }
    }
    return current_node; // bf == 1,,0,-1 and therefore balanced
}

template<typename T>
Node<T> *AvlTree<T>::RotateLeft(Node<T> *node) {

    Node<T>* other = node->getRight();
    node->setRight(other->getLeft());
    other->setLeft(node);
    update(node);
    update(other);
    return other;

}
template<typename T>
Node<T>* AvlTree<T>::RotateRight(Node<T> *node) {

    Node<T>* other = node->getLeft();
    node->setLeft(other->getRight());
    other->setRight(node);
    update(node);
    update(other);
    return other;

}

template<typename T>
Node<T> *AvlTree<T>::LLR(Node<T> *node) {
    return RotateRight(node);
}

template<typename T>
Node<T> *AvlTree<T>::LRR(Node<T> *node) {
    node->setLeft(RotateLeft(node->getLeft()));
    return LLR(node);
}

template<typename T>
Node<T> *AvlTree<T>::RLR(Node<T> *node) {
    node->setRight(RotateRight(node->getRight()));
    return RRR(node);
}

template<typename T>
Node<T> *AvlTree<T>::RRR(Node<T> *node) {
    return RotateLeft(node);
}

template<typename T>
void AvlTree<T>::print() {
    if (this->_root == nullptr)
    {
        return;
    }

    std::cout << this->_root->getData() << std::endl;
    printSubtree(this->_root, "");
    std::cout << std::endl;
}

template<typename T>
void AvlTree<T>::printSubtree(Node<T> *root, const string &prefix) {
    if (root == nullptr)
    {
        return;
    }

    bool hasLeft = (root->getLeft()!= nullptr);
    bool hasRight = (root->getRight() != nullptr);

    if (!hasLeft && !hasRight)
    {
        return;
    }

    cout << prefix;
    cout << ((hasLeft  && hasRight) ? "|__ " : "");
    cout << ((!hasLeft && hasRight) ? "L__ " : "");

    if (hasRight)
    {
        bool printStrand = (hasLeft && (root->getRight()->getRight() != nullptr || root->getRight()->getLeft() != nullptr));
        string newPrefix = prefix + (printStrand ? "|   " : "    ");
        cout << root->getRight()->getData() << endl;
        printSubtree(root->getRight(), newPrefix);
    }

    if (hasLeft)
    {
        cout << (hasRight ? prefix : "") << "L__ " << root->getLeft()->getData() << endl;
        printSubtree(root->getLeft(), prefix + "    ");
    }
}

template<typename T>
bool AvlTree<T>::remove(const T &value,int (*compare)(T,T)) {
    if(compare == nullptr){
        return false;
    }
    if(find(value,compare) == nullptr){
        return false;
    }
    if(find(value,compare) != nullptr){
        this->_root = remove(this->_root ,value,compare);
        return true;
    }
    return false;
}

template<typename T>
Node<T> *AvlTree<T>::remove(Node<T> *current_node, const T &value,int (*compare)(T,T)) {
    if(current_node == nullptr || compare == nullptr){// no value do nothing
        return nullptr;
    }
    if(compare(value , current_node->getData()) == 1){ // value in left tree
        current_node->setLeft(remove(current_node->getLeft(), value,compare));
    }
    else if(compare( value , current_node->getData()) == -1){ // value in right tree
        current_node->setRight(remove(current_node->getRight(), value,compare));
    }
    else{ // found the value and here the remove happened
        if(current_node->getLeft() == nullptr){ // left / no child case
            Node<T>* temp = current_node->getRight();

            if(temp == nullptr){// no child case
                delete current_node;
                return nullptr;
            }
            else {// right child case

                current_node->setRight(nullptr);
                delete current_node;
            }

            return temp;
        }
        else if(current_node->getRight() == nullptr){// left child case

            Node<T>* temp = current_node->getLeft();
            current_node->setLeft(nullptr);
            delete current_node;
            return temp;
        }
        else {
            if(current_node->getLeft()->getheight() > current_node->getRight()->getheight()){ // left heavy node
                const T& successorValue = findMax(current_node->getLeft()); // biggest from all smaller values
                current_node->setData(successorValue);
                current_node->setLeft(remove(current_node->getLeft(), successorValue,compare));
            }
            else{
                const T& successorValue = findMin(current_node->getRight());//smaller from all larger values
                current_node->setData(successorValue);
                current_node->setRight(remove(current_node->getRight(), successorValue,compare));
            }

        }
    }
    update(current_node);
    return balance(current_node);
}

template<typename T>
const T &AvlTree<T>::findMax(Node<T> *node) {
    while(node->getRight()!= nullptr){
        node = node->getRight();
    }
    return node->getData();
}

template<typename T>
const T &AvlTree<T>::findMin(Node<T> *node) {
    while (node->getLeft()!= nullptr){
        node = node->getLeft();
    }
    return node->getData();
}

template<typename T>
AvlTree<T>::~AvlTree() {
    delete this->_root;
}

template<typename T>
const T &AvlTree<T>::findMax() {
    return this->findMax(this->_root);
}

template<typename T>
const T &AvlTree<T>::findMin() {
    return this->findMin(this->_root);
}

template<typename T>
Node<T> *
AvlTree<T>::insert(const T &value, Node<T> *current_node, int (*compare)(T, T), const T &left, const T &right) {
    if(compare == nullptr){
        return  nullptr;
    }
    if (current_node == nullptr){
        Node<T>* new_node = nullptr;
        try {
            new_node = new Node<T>(value);
        }
        catch (const std::bad_alloc&) {
            delete new_node;
            return nullptr;
        }
        return new_node;
    }
    if(compare(current_node->getData() , value)==-1){//current_node->getData() > value
        left = current_node->getData();
        current_node->setLeft(insert(value, current_node->getLeft() ,compare));
    }
    else{
        right = current_node->getData();
        current_node->setRight(insert(value, current_node->getRight(),compare));
    }
    update(current_node);
    return  balance(current_node);
}


template<typename T>
Node<T>::Node(const T &data) {
    this->_data = data;
    this->_right = nullptr;
    this->_left = nullptr;
    this->_parent = nullptr;
    this->_height= 1;
    this->BF = 0;
}

template<typename T>
Node<T>::Node(const T &data, Node<T> *left, Node<T> *right, Node<T> parent) {
    this->_data = data;
    this->_left = left;
    this->_right = right;
    this->_parent = parent;
    this->_height = std::max(left->_height,right->_height)+1;
    this->BF = right->_height - left->_height;
}

template<typename T>
Node<T>::~Node() {
    if(this->_right != nullptr){
        delete this->_right;
    }
    if(this->_left != nullptr){
        delete this->_left;
    }
}

template<typename T>
const T &Node<T>::getData() {
    return this->_data;
}

template<typename T>
Node<T> *Node<T>::getLeft() {
    return this->_left;
}
template<typename T>
Node<T> *Node<T>::getRight() {
    return this->_right;
}

template<typename T>
Node<T> *Node<T>::getParent() {
    return this->_parent;
}

template<typename T>
void Node<T>::setLeft(Node<T> *left) {
    this->_left = left;
}

template<typename T>
void Node<T>::setRight(Node<T> *right) {
    this->_right = right;
}

template<typename T>
void Node<T>::setParent(Node<T> *parent) {
    this->_parent = parent ;
}

template<typename T>
void Node<T>::setData(const T &data) {
    this->_data = data;
}

template<typename T>
void Node<T>::print() const {
    std::cout << this->_data << std::endl;
}

template<typename T>
int Node<T>::getheight() {
    return this->_height;
}

template<typename T>
void Node<T>::setheight(int height) {
    this->_height = height;
}

template<typename T>
int Node<T>::getBF() {
    return this->BF;
}

template<typename T>
void Node<T>::setBF(int BF1) {
    this->BF = BF1;
}


#endif //DATA_STRUCT_EX1_AVLTREE_H