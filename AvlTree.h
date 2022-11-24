#ifndef DATA_STRUCT_EX1_AVLTREE_H
#define DATA_STRUCT_EX1_AVLTREE_H
#include <iostream>

template<typename T>
class Node{
//TODO:change all private members to _
    private:
    T data;
    Node<T>* left;
    Node<T>* right;
    Node<T>* parent;
    public:

    Node(T data) {
        this->data = data;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
    Node(T data, Node<T>* left, Node<T>* right, Node<T> parent) {
        this->data = data;
        this->left = left;
        this->right = right;
        this->parent = parent;
    }
    ~Node(){
        if(left!= nullptr){
            delete left;
        }
        if(right!= nullptr){
            delete right;
        }
        delete data;
    }
    const T& getData() { return data; }
    Node<T>* getLeft() { return left; }
    Node<T>* getRight() { return right; }
    Node<T>* getParent() { return parent; }
    void setLeft(Node<T>* left) { this->left = left; }
    void setRight(Node<T>* right) { this->right = right; }
    void setParent(Node<T>* parent) { this->parent = parent; }
    void setData(T data) { this->data = data; }
    void print() const { std::cout << this->data<<std::endl; }

};
template<typename T>
class  AvlTree{

    private:
    Node<T>* root;
    Node<T>* find(const T& value,Node<T>* current_node);
    public:
    AvlTree(){
        root=nullptr;
    }
    ~AvlTree(){
        if(root!=nullptr){
            delete root;
            root=nullptr;
    }
    bool add(const T& value);
    Node<T>* getRoot();
    void print();//prints the tree in InOrder format
    Node<T>* find(const T& value); // calls private find with the root
    int hight(Node<T>* node);
    int BalanceFactor(Node<T>* node);
    void RotateRight(Node<T>* node);
    void RotateLeft(Node<T>* node);
    void remove(const T& value);
    //TODO: operators

    }
#endif //DATA_STRUCT_EX1_AVLTREE_H