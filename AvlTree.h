#ifndef DATA_STRUCT_EX1_AVLTREE_H
#define DATA_STRUCT_EX1_AVLTREE_H
#include <iostream>
using namespace std;


template<typename T>
class Node{
//TODO:change all private members to _
    private:
    T _data;
    int _height{};
    int BF{}; // the balanceFactor of the tree note : _bf is reseved by the language
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


    Node<T>* find(Node<T>* current_node,const T& value);
    Node<T>* insert(const T& value,Node<T>* current_node); // the recursive private function that inserts the node
    Node<T>* remove(Node<T>* current_node,const T& value); // the recursive private function
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
    ~AvlTree() = default; // should check if this works
    bool add(const T& value);
    Node<T>* getRoot();
    void print();//prints the tree (copied from the internet)
    Node<T>* find(const T& value); // calls private find with the root
    int height(Node<T>* node);
    int BalanceFactor(Node<T>* node);
    Node<T>* RotateRight(Node<T>* node);
    Node<T>* RotateLeft(Node<T>* node);
    bool remove(const T& value);
    //TODO: operators

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
Node<T> *AvlTree<T>::find(const T &value) {
    if(this->_root == nullptr){
        return nullptr;
    }
    Node<T>* result = find(this->_root, value);
    if(result->getData() != value){ // if value is not  in the tree return  nullptr instead
        return nullptr;
    }
    return result;
}

template<typename T>
Node<T> *AvlTree<T>::find( Node<T> *current_node,const T &value) { // returns the closest node
    if(current_node == nullptr){
        return nullptr;
    }
    else if (current_node->getData() > value && current_node->getLeft() != nullptr){
        return find(current_node->getLeft() , value);
    }
    else if (current_node->getData() < value && current_node->getRight() != nullptr){
        return find(current_node->getRight() , value);
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
bool AvlTree<T>::add(const T &value) {
    if (this->_root == nullptr){
        try{
            this->_root = new Node<T>(value);

        }
        catch(std::bad_alloc&){
            return false;
        }
        return true;
    }
    Node<T>* temp = this->find(value);
    if(temp != nullptr){
        return false; // already added
    }
    this->_root = insert(value, this->_root);
    return true;

}

template<typename T>
Node<T> *AvlTree<T>::insert(const T &value, Node<T> *current_node) {
    if (current_node == nullptr){
        Node<T>* new_node = nullptr;
        try {
            new_node = new Node<T>(value);
        }
        catch (const std::bad_alloc&) {
            return nullptr;
        }
        return new_node;
    }
    if(current_node->getData() > value){
        current_node->setLeft(insert(value, current_node->getLeft()));
    }
    else{
        current_node->setRight(insert(value, current_node->getRight()));
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
    node->setLeft(RotateLeft(node));
    return LLR(node);
}

template<typename T>
Node<T> *AvlTree<T>::RLR(Node<T> *node) {
    node->setRight(RotateRight(node));
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
bool AvlTree<T>::remove(const T &value) {
    if(find(value) == nullptr){
        return false;
    }
    if(find(value) != nullptr){
        this->_root = remove(this->_root ,value);
        return true;
    }
    return false;
}

template<typename T>
Node<T> *AvlTree<T>::remove(Node<T> *current_node, const T &value) {
    if(current_node == nullptr){// no value do nothing
        return nullptr;
    }
    if(value < current_node->getData()){ // value in left tree
        current_node->setLeft(remove(current_node->getLeft(), value));
    }
    else if(value > current_node->getData()){ // value in right tree
        current_node->setRight(remove(current_node->getRight(), value));
    }
    else{ // found the value and here the remove happened
        if(current_node->getLeft() == nullptr){ // left / np child case
            Node<T>* temp = current_node->getRight();
            if(temp == nullptr){// no child case
                if(current_node==this->_root)
                    this->_root = nullptr;
                delete current_node;
                return nullptr;
            }
            else {// right child case
                current_node->setLeft(temp->getLeft());
                current_node->setRight(temp->getRight());
                current_node->setData(temp->getData());

                temp->setRight(nullptr);
                temp->setLeft(nullptr);
                delete temp;
            }
            return current_node->getRight();
        }
        else if(current_node->getRight() == nullptr){// left child case

            Node<T>* temp = current_node->getLeft();

            current_node->setLeft(temp->getLeft());
            current_node->setRight(temp->getRight());
            current_node->setData(temp->getData());

            temp->setRight(nullptr);
            temp->setLeft(nullptr);
            delete temp;
            return current_node->getLeft();
        }
        else {
            if(current_node->getLeft()->getheight() > current_node->getRight()->getheight()){ // left heavy node
                const T& successorValue = findMax(current_node->getLeft()); // biggest from all smaller values
                current_node->setData(successorValue);
                current_node->setLeft(remove(current_node->getLeft(), successorValue));
            }
            else{
                const T& successorValue = findMin(current_node->getRight());//smaller from all larger values
                current_node->setData(successorValue);
                current_node->setRight(remove(current_node->getRight(), successorValue));
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