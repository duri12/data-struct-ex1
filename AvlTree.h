#ifndef DATA_STRUCT_EX1_AVLTREE_H
#define DATA_STRUCT_EX1_AVLTREE_H
#include <iostream>

template<typename T>
class Node{
//TODO:change all private members to _
    private:
    T _data;
    int _height;
    Node<T>* _left;
    Node<T>* _right;
    Node<T>* _parent;
    public:

    explicit Node(const T& data) ;
    Node(const T& data, Node<T>* left, Node<T>* right, Node<T> parent) ;
    ~Node();
    const T& getData();
    int getheight();
    Node<T>* getLeft();
    Node<T>* getRight();
    Node<T>* getParent();
    void setLeft(Node<T>* left);
    void setRight(Node<T>* right);
    void setParent(Node<T>* parent);
    void setData(const T& data);
    void setheight(int height);
    void print() const ;

};

template<typename T>
class  AvlTree{

    private:
    Node<T>* _root;
    Node<T>* find(const T& value,Node<T>* current_node);
    public:
    AvlTree();
    ~AvlTree() = default; // should check if this works
    bool add(const T& value);
    Node<T>* getRoot();
    void print();//prints the tree in InOrder format
    Node<T>* find(const T& value); // calls private find with the root
    int height(Node<T>* node);
    int BalanceFactor(Node<T>* node);
    void RotateRight(Node<T>* node);
    void RotateLeft(Node<T>* node);
    void remove(const T& value);
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
    return find(this->_root, value);
}

template<typename T>
Node<T> *AvlTree<T>::find(const T &value, Node<T> *current_node) {
    if(current_node == nullptr){
        return nullptr;
    }
    else if(current_node->Node::getData() == value){
        return current_node;
    }
    else if (current_node->Node::getData() > value){
        return find(current_node->getLeft() , value);
    }
    return find(current_node->getRight() , value);
}

template<typename T>
int AvlTree<T>::height(Node<T> *node) {
    if(node == nullptr)
        return 0;
    return node->Node::getHeight();
}

template<typename T>
int AvlTree<T>::BalanceFactor(Node<T> *node) {
    if(node == nullptr)
        return 0;
    return node->getLeft()->getheight() - node->getRight()->getheight();
}

template<typename T>
void AvlTree<T>::RotateRight(Node<T> *node) {

}


template<typename T>
Node<T>::Node(const T &data) {
    this->_data = data;
    this->_right = nullptr;
    this->_left = nullptr;
    this->_parent = nullptr;
    this->_height= 1;
}

template<typename T>
Node<T>::Node(const T &data, Node<T> *left, Node<T> *right, Node<T> parent) {
    this->_data = data;
    this->_left = left;
    this->_right = right;
    this->_parent = parent;
    this->_height = std::max(left->_height,right->_height)+1;
}

template<typename T>
Node<T>::~Node() {
    if(this->_right != nullptr){
        delete this->_right;
    }
    if(this->_left != nullptr){
        delete this->_left;
    }
    delete this->_data;
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


#endif //DATA_STRUCT_EX1_AVLTREE_H