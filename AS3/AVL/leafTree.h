#ifndef LEAFTREE_H
#define LEAFTREE_H

#include <iostream>

template<typename T, typename B>
class node {
  public:
    T key;
    B val;
    node* left;
    node* right;
    bool leaf;
    node(T nkey, B nval, node* nleft = nullptr, node* nright = nullptr, bool nleaf = false);
};

template<typename T, typename B>
class leafTree {
    node<T,B>* root;
    size_t size;

  public:
    leafTree(node<T,B>* nnode = nullptr);
    node<T, B>* deleteNode(T key);
    node<T, B>* find(T key);
    node<T, B>* insert(T key, B val);
    node<T, B>*& getRootRef() { return root; }
    node<T, B>* getRoot() const { return root; }
    void setRoot(node<T, B>* newRoot) { root = newRoot; }
    size_t getSize() const { return size; }
    void setSize(size_t newSize) { size = newSize; }
    void incrementSize() { size++; }
    void decrementSize() { if (size > 0) size--; }

  private:
    node<T, B>* findNode(T key);
    node<T, B>* findParent(T key, node<T, B>* actual, node<T, B>* parent);
    node<T, B>* findNode(T key, node<T, B>* actual);
    node<T, B>* find(T key, node<T, B>* actual);
};

template<typename T, typename B>
node<T,B>::node(T nkey, B nval, node* nleft, node* nright, bool nleaf) : 
key(nkey), val(nval), left(nleft), right(nright), leaf(nleaf) {}

template<typename T, typename B>
leafTree<T,B>::leafTree(node<T,B>* nnode) : root(nnode), size(nnode ? 1 : 0) {}

template<typename T, typename B>
node<T, B>* leafTree<T,B>::findNode(T key) 
{ 
  return findNode(key, root); 
}

template<typename T, typename B>
node<T, B>* leafTree<T,B>::insert(T key, B val) 
{
  if (root == nullptr) {
    root = new node<T, B>(key, val);
    size++;
    return root;
  }

  node<T, B>* parent = findNode(key);
  if (!parent) return nullptr;

  node<T, B>* old_node = new node<T,B>(parent->key, parent->val);
  node<T, B>* new_node = new node<T,B>(key, val);

  if (parent->key < key) {
    parent->key = key;
    parent->right = new_node;
    parent->left = old_node;
  } else {
    parent->right = old_node;
    parent->left = new_node;
  }
  size++;

  parent->leaf = false;
  old_node->leaf = true;
  new_node->leaf = true;

  return new_node;
}

template<typename T, typename B>
node<T, B>* leafTree<T,B>::findNode(T key, node<T, B>* actual) 
{
  if (actual == nullptr)
    return nullptr;

  if (actual->key <= key) {
    if (actual->right == nullptr) 
      return actual;
    return findNode(key, actual->right);
  } else {
    if (actual->left == nullptr) 
      return actual;
    return findNode(key, actual->left);
  }
}

template<typename T, typename B>
node<T, B>* leafTree<T,B>::find(T key) 
{
  node<T, B>* result = find(key, root);
  if (result->key == key) {
    return result;
  }
  return nullptr;
}

template<typename T, typename B>
node<T, B>* leafTree<T,B>::find(T key, node<T, B>* actual) 
{
  if (actual == nullptr)
    return nullptr;

  if (actual->key <= key and !actual->leaf) {
    if (actual->right == nullptr) 
      return actual;
    return find(key, actual->right);
  } else if (actual->key > key and !actual->leaf){
    if (actual->left == nullptr) 
      return actual;
    return find(key, actual->left);
  } else {
    return actual;
  }
}

template<typename T, typename B>
node<T, B>* leafTree<T,B>::findParent(T key, node<T, B>* actual, node<T, B>* parent) 
{
  if (actual == nullptr)
    return nullptr;

  if (actual->leaf && actual->key == key) {
    return parent;
  }

  if (actual->leaf) {
    return nullptr;
  }

  if (key <= actual->key) {
    return findParent(key, actual->left, actual);
  } else {
    return findParent(key, actual->right, actual);
  }
}

template<typename T, typename B>
node<T, B>* leafTree<T,B>::deleteNode(T key) 
{
  if (root == nullptr) {
    return nullptr;
  }

  // Caso especial: árbol con un solo nodo
  if (root->left == nullptr && root->right == nullptr) {
    if (root->key == key) {
      node<T, B>* deleted = root;
      root = nullptr;
      size--;
      return deleted;
    } else {
      return nullptr;
    }
  }

  // Variables para navegar
  node<T, B>* tmp_node = root;
  node<T, B>* upper_node = nullptr;
  node<T, B>* other_node = nullptr;

  // Navegar hasta encontrar el nodo hoja
  while (tmp_node->right != nullptr) {
    upper_node = tmp_node;
    if (key < tmp_node->key) {
      tmp_node = upper_node->left;
      other_node = upper_node->right;
    } else {
      tmp_node = upper_node->right;
      other_node = upper_node->left;
    }
  }

  // Verificar si encontramos el nodo a eliminar
  if (tmp_node->key != key) {
    return nullptr;
  }

  // El nodo padre (upper_node) absorbe al hermano (other_node)
  upper_node->key = other_node->key;
  upper_node->val = other_node->val;
  upper_node->left = other_node->left;
  upper_node->right = other_node->right;
  upper_node->leaf = other_node->leaf;

  node<T, B>* deleted_object = tmp_node;
  delete other_node;
  size--;

  return deleted_object;
}

#endif
