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
    node* parent;
    bool leaf;
    node(T nkey, B nval, node* nleft = nullptr, node* nright = nullptr, node* nparent = nullptr, bool nleaf = false);
};

template<typename T, typename B>
class leafTree {
  node<T,B>* root;
  size_t size;

  public:
  leafTree(node<T,B>* nnode = nullptr);
  ~leafTree();
  node<T, B>* deleteNode(T key);
  node<T, B>* find(T key);
  node<T, B>* insert(T key, B val);
  size_t getSize() const { return size; }

  private:
  node<T, B>* findNode(T key);
  node<T, B>* findParent(T key, node<T, B>* actual, node<T, B>* parent);
  node<T, B>* findNode(T key, node<T, B>* actual);
  node<T, B>* find(T key, node<T, B>* actual);
  void destroyTree(node<T, B>* actual);
};

template<typename T, typename B>
node<T,B>::node(T nkey, B nval, node* nleft, node* nright, node* nparent, bool nleaf) : 
  key(nkey), val(nval), left(nleft), right(nright), parent(nparent), leaf(nleaf) {}

  template<typename T, typename B>
  leafTree<T,B>::leafTree(node<T,B>* nnode) : root(nnode), size(nnode ? 1 : 0) {}

  template<typename T, typename B>
  leafTree<T,B>::~leafTree() {
    destroyTree(root);
  }

template<typename T, typename B>
void leafTree<T,B>::destroyTree(node<T, B>* actual) 
{
  if (actual != nullptr) {
    destroyTree(actual->left);
    destroyTree(actual->right);
    delete actual;
  }
}

template<typename T, typename B>
node<T, B>* leafTree<T,B>::findNode(T key) 
{ 
  return findNode(key, root); 
}

template<typename T, typename B>
node<T, B>* leafTree<T,B>::insert(T key, B val) 
{
  if (root == nullptr) {
    root = new node<T, B>(key, val, nullptr, nullptr, nullptr, true);
    size++;
    return root;
  }

  node<T, B>* parent = findNode(key);
  if (!parent) return nullptr;

  node<T, B>* old_node = new node<T,B>(parent->key, parent->val, nullptr, nullptr, parent, true);
  node<T, B>* new_node = new node<T,B>(key, val, nullptr, nullptr, parent, true);

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
  if (result && result->key == key) {
    return result;
  }
  return nullptr;
}

template<typename T, typename B>
node<T, B>* leafTree<T,B>::find(T key, node<T, B>* actual) 
{
  if (actual == nullptr)
    return nullptr;

  if (actual->key <= key && !actual->leaf) {
    if (actual->right == nullptr) 
      return actual;
    return find(key, actual->right);
  } else if (actual->key > key && !actual->leaf){
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

  node<T, B>* tmp_node = root;
  node<T, B>* upper_node = nullptr;
  node<T, B>* other_node = nullptr;

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

  if (tmp_node->key != key) {
    return nullptr;
  }

  upper_node->key = other_node->key;
  upper_node->val = other_node->val;
  upper_node->left = other_node->left;
  upper_node->right = other_node->right;
  upper_node->leaf = other_node->leaf;

  if (other_node->left) other_node->left->parent = upper_node;
  if (other_node->right) other_node->right->parent = upper_node;

  node<T, B>* deleted_object = tmp_node;
  delete other_node;
  size--;

  return deleted_object;
}

#endif
