#ifndef RB_NODE_TREE_H
#define RB_NODE_TREE_H

#include <iostream>
#include <utility>
template <typename T, typename B>
class RBNodeTree {
  enum Color { RED, BLACK };

  struct Node {
    T key;
    B val;
    Color color;
    Node *left;
    Node *right;
    Node *parent;

    Node(const T &k = T(), const B &v = B(), Color c = RED,
         Node *l = nullptr, Node *r = nullptr, Node *p = nullptr)
        : key(k), val(v), color(c), left(l), right(r), parent(p) {}
  };

  Node *root; 
  Node *nil; 
  size_t sz;

 public:
  RBNodeTree();
  ~RBNodeTree();

  void insert(const T &key, const B &val);
  bool erase (const T &key);
  Node *find (const T &key) const;
  size_t size() const { return sz; }

  Node * _test_root() const { return root; }

 private:
  void destroy(Node *x);
  void leftRotate (Node *x);
  void rightRotate(Node *y);
  void insertFix (Node *z);
  void deleteFix (Node *x);
  Node *minimum(Node *x) const;
  void transplant (Node *u, Node *v);
};

// imp
template <typename T, typename B>
RBNodeTree<T,B>::RBNodeTree() {
  nil = new Node();
  nil->color = BLACK;
  nil->left = nil->right = nil->parent = nil;
  root = nil;
  sz = 0;
}

template <typename T, typename B>
RBNodeTree<T,B>::~RBNodeTree() {
  destroy(root);
  delete nil;
}

template <typename T, typename B>
void RBNodeTree<T,B>::destroy(Node *x) {
  if (x == nil) return;
  destroy(x->left);
  destroy(x->right);
  delete x;
}

// rot
template <typename T, typename B>
void RBNodeTree<T,B>::leftRotate(Node *x) {
  Node *y = x->right;
  x->right = y->left;
  if (y->left != nil) y->left->parent = x;
  y->parent = x->parent;
  if (x->parent == nil)
    root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;
  y->left = x;
  x->parent = y;
}

template <typename T, typename B>
void RBNodeTree<T,B>::rightRotate(Node *y) {
  Node *x = y->left;
  y->left = x->right;
  if (x->right != nil) x->right->parent = y;
  x->parent = y->parent;
  if (y->parent == nil)
    root = x;
  else if (y == y->parent->left)
    y->parent->left = x;
  else
    y->parent->right = x;
  x->right = y;
  y->parent = x;
}

// ins
template <typename T, typename B>
void RBNodeTree<T,B>::insert(const T &key, const B &val) {
  Node *z = new Node(key,val,RED,nil,nil,nil);
  Node *y = nil;
  Node *x = root;
  while (x != nil) {
    y = x;
    if (key < x->key) x = x->left;
    else if (x->key < key) x = x->right;
    else { x->val = val; delete z; return; }
  }
  z->parent = y;
  if (y == nil) root = z;
  else if (key < y->key) y->left = z;
  else y->right = z;

  ++sz;
  insertFix(z);
}

// reb
template <typename T, typename B>
void RBNodeTree<T,B>::insertFix(Node *z) {
  while (z->parent->color == RED) {
    if (z->parent == z->parent->parent->left) {
      Node *y = z->parent->parent->right; 
      if (y->color == RED) { // caso 1
        z->parent->color = BLACK;
        y->color = BLACK;
        z->parent->parent->color = RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->right) { // caso 2
          z = z->parent;
          leftRotate(z);
        }
        z->parent->color = BLACK; // caso 3
        z->parent->parent->color = RED;
        rightRotate(z->parent->parent);
      }
    } else { // simétrico
      Node *y = z->parent->parent->left;
      if (y->color == RED) {
        z->parent->color = BLACK;
        y->color = BLACK;
        z->parent->parent->color = RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->left) {
          z = z->parent;
          rightRotate(z);
        }
        z->parent->color = BLACK;
        z->parent->parent->color = RED;
        leftRotate(z->parent->parent);
      }
    }
  }
  root->color = BLACK;
}

// sear
template <typename T, typename B>
typename RBNodeTree<T,B>::Node* RBNodeTree<T,B>::find(const T &key) const {
  Node *x = root;
  while (x != nil) {
    if (key == x->key) return x;
    x = (key < x->key) ? x->left : x->right;
  }
  return nullptr;
}

// erase
template <typename T, typename B>
bool RBNodeTree<T,B>::erase(const T &key) {
  Node *z = find(key);
  if (!z) return false;

  Node *y = z;
  Node *x;
  Color y_original = y->color;

  if (z->left == nil) {
    x = z->right;
    transplant(z, z->right);
  } else if (z->right == nil) {
    x = z->left;
    transplant(z, z->left);
  } else {
    y = minimum(z->right);
    y_original = y->color;
    x = y->right;
    if (y->parent == z) x->parent = y;
    else {
      transplant(y, y->right);
      y->right = z->right; y->right->parent = y;
    }
    transplant(z, y);
    y->left = z->left; y->left->parent = y;
    y->color = z->color;
  }
  delete z;
  --sz;

  if (y_original == BLACK) deleteFix(x);
  return true;
}

// reb
template <typename T, typename B>
void RBNodeTree<T,B>::deleteFix(Node *x) {
  while (x != root && x->color == BLACK) {
    if (x == x->parent->left) {
      Node *w = x->parent->right;
      if (w->color == RED) { // caso 1
        w->color = BLACK;
        x->parent->color = RED;
        leftRotate(x->parent);
        w = x->parent->right;
      }
      if (w->left->color == BLACK && w->right->color == BLACK) { // caso 2
        w->color = RED;
        x = x->parent;
      } else {
        if (w->right->color == BLACK) { // caso 3
          w->left->color = BLACK;
          w->color = RED;
          rightRotate(w);
          w = x->parent->right;
        }
        w->color = x->parent->color; // caso 4
        x->parent->color = BLACK;
        w->right->color = BLACK;
        leftRotate(x->parent);
        x = root;
      }
    } else { // simétrico
      Node *w = x->parent->left;
      if (w->color == RED) {
        w->color = BLACK;
        x->parent->color = RED;
        rightRotate(x->parent);
        w = x->parent->left;
      }
      if (w->right->color == BLACK && w->left->color == BLACK) {
        w->color = RED;
        x = x->parent;
      } else {
        if (w->left->color == BLACK) {
          w->right->color = BLACK;
          w->color = RED;
          leftRotate(w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = BLACK;
        w->left->color = BLACK;
        rightRotate(x->parent);
        x = root;
      }
    }
  }
  x->color = BLACK;
}

template <typename T, typename B>
void RBNodeTree<T,B>::transplant(Node *u, Node *v) {
  if (u->parent == nil) root = v;
  else if (u == u->parent->left) u->parent->left = v;
  else u->parent->right = v;
  v->parent = u->parent;
}

template <typename T, typename B>
typename RBNodeTree<T,B>::Node* RBNodeTree<T,B>::minimum(Node *x) const {
  while (x->left != nil) x = x->left;
  return x;
}

#endif /* RB_NODE_TREE_H */
