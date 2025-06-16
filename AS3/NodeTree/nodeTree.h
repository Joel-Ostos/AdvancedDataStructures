#ifndef NODETREE_H
#define NODETREE_H

#include <iostream>
#include <algorithm>

template<typename T, typename B>
class nodeT {
  public:
    T key;
    B val;
    nodeT* left;
    nodeT* right;
    nodeT* parent;  

    nodeT(T nkey, B nval, nodeT* nleft = nullptr, nodeT* nright = nullptr, nodeT* nparent = nullptr);
};

template<typename T, typename B>
class nodeTree {
  nodeT<T,B>* root;
  size_t size;

  public:
  nodeTree(nodeT<T,B>* nnode = nullptr);
  ~nodeTree();

  nodeT<T, B>* insert(T key, B val);
  nodeT<T, B>* find(T key);
  nodeT<T, B>* deleteNode(T key);

  nodeT<T, B>* getMin();
  nodeT<T, B>* getMax();
  size_t getSize() const { return size; }

  private:
  nodeT<T, B>* insert(T key, B val, nodeT<T, B>* actual, nodeT<T, B>* parent);

  nodeT<T, B>* find(T key, nodeT<T, B>* actual);

  nodeT<T, B>* deleteNode(T key, nodeT<T, B>* actual);
  nodeT<T, B>* findMin(nodeT<T, B>* actual);
  nodeT<T, B>* findMax(nodeT<T, B>* actual);

  void destroyTree(nodeT<T, B>* actual);
};

template<typename T, typename B>
nodeT<T,B>::nodeT(T nkey, B nval, nodeT* nleft, nodeT* nright, nodeT* nparent) : 
  key(nkey), val(nval), left(nleft), right(nright), parent(nparent) {}

  template<typename T, typename B>
  nodeTree<T,B>::nodeTree(nodeT<T,B>* nnode) : root(nnode), size(nnode ? 1 : 0) {}

  template<typename T, typename B>
  nodeTree<T,B>::~nodeTree() {
    destroyTree(root);
  }

template<typename T, typename B>
void nodeTree<T,B>::destroyTree(nodeT<T, B>* actual) 
{
  if (actual != nullptr) {
    destroyTree(actual->left);
    destroyTree(actual->right);
    delete actual;
  }
}

template<typename T, typename B>
nodeT<T, B>* nodeTree<T,B>::insert(T key, B val) 
{
  root = insert(key, val, root, nullptr);
  return root;
}

template<typename T, typename B>
nodeT<T, B>* nodeTree<T,B>::insert(T key, B val, nodeT<T, B>* actual, nodeT<T, B>* parent) 
{
  if (actual == nullptr) {
    size++;
    return new nodeT<T, B>(key, val, nullptr, nullptr, parent);
  }

  if (key == actual->key) {
    actual->val = val;
    return actual;
  }

  if (key < actual->key) {
    actual->left = insert(key, val, actual->left, actual);
  } else {
    actual->right = insert(key, val, actual->right, actual);
  }

  return actual;
}

template<typename T, typename B>
nodeT<T, B>* nodeTree<T,B>::find(T key) 
{
  return find(key, root);
}

template<typename T, typename B>
nodeT<T, B>* nodeTree<T,B>::find(T key, nodeT<T, B>* actual) 
{
  if (actual == nullptr) {
    return nullptr;
  }

  if (key == actual->key) {
    return actual;
  }

  if (key < actual->key) {
    return find(key, actual->left);
  } else {
    return find(key, actual->right);
  }
}

template<typename T, typename B>
nodeT<T, B>* nodeTree<T,B>::deleteNode(T key) 
{
  root = deleteNode(key, root);
  return root;
}

template<typename T, typename B>
nodeT<T, B>* nodeTree<T,B>::deleteNode(T key, nodeT<T, B>* actual) 
{
  if (actual == nullptr) {
    return nullptr;
  }

  if (key < actual->key) {
    actual->left = deleteNode(key, actual->left);
  } else if (key > actual->key) {
    actual->right = deleteNode(key, actual->right);
  } else {
    size--;

    if (actual->left == nullptr && actual->right == nullptr) {
      delete actual;
      return nullptr;
    }

    if (actual->left == nullptr) {
      nodeT<T, B>* temp = actual->right;
      temp->parent = actual->parent;
      delete actual;
      return temp;
    }
    if (actual->right == nullptr) {
      nodeT<T, B>* temp = actual->left;
      temp->parent = actual->parent;
      delete actual;
      return temp;
    }

    nodeT<T, B>* successor = findMin(actual->right);

    actual->key = successor->key;
    actual->val = successor->val;

    actual->right = deleteNode(successor->key, actual->right);
    size++; 
  }

  return actual;
}

template<typename T, typename B>
nodeT<T, B>* nodeTree<T,B>::findMin(nodeT<T, B>* actual) 
{
  if (actual == nullptr) {
    return nullptr;
  }

  while (actual->left != nullptr) {
    actual = actual->left;
  }
  return actual;
}

template<typename T, typename B>
nodeT<T, B>* nodeTree<T,B>::findMax(nodeT<T, B>* actual) 
{
  if (actual == nullptr) {
    return nullptr;
  }

  while (actual->right != nullptr) {
    actual = actual->right;
  }
  return actual;
}

template<typename T, typename B>
nodeT<T, B>* nodeTree<T,B>::getMin() 
{
  return findMin(root);
}

template<typename T, typename B>
nodeT<T, B>* nodeTree<T,B>::getMax() 
{
  return findMax(root);
}

#endif
