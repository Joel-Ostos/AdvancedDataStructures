#ifndef RB_LEAF_TREE_H
#define RB_LEAF_TREE_H

#include <iostream>
#include <stack>

template <typename T, typename B>
class RBLeafTree {
  enum Color { RED, BLACK };

  struct Node {
    T key; 
    B val;
    Node *left;
    Node *right;
    Node *parent;
    bool leaf; 
    Color color;

    Node(const T& k,
         const B& v,
         Node* l = nullptr,
         Node* r = nullptr,
         Node* p = nullptr,
         bool lf = true,
         Color c = RED)
      : key{k}, val{v}, left{l}, right{r}, parent{p}, leaf{lf}, color{c} {}
  };

  Node *root {nullptr};
  size_t sz {0};

 public:
  ~RBLeafTree() { destroy(root); }

  void insert(const T& key, const B& val);
  bool erase(const T& key);
  const B* find(const T& key) const;
  size_t size() const { return sz; }

 private:
  void destroy(Node* x);

  Node* findLeaf(const T& key) const;

  void leftRotate (Node* x);
  void rightRotate(Node* y);

  void insertFix(Node* current, std::stack<Node*>& ancestors);
  void deleteFix(Node* current);

  Node* sibling(Node* x) const {
    return (x == x->parent->left) ? x->parent->right : x->parent->left;
  }

  static Color nodeColor(Node* p) { return p ? p->color : BLACK; }
};

// imp
template <typename T, typename B>
void RBLeafTree<T,B>::destroy(Node* x) {
  if (!x) return;
  destroy(x->left);
  destroy(x->right);
  delete x;
}

// find
template <typename T, typename B>
const B* RBLeafTree<T,B>::find(const T& key) const {
  Node* leaf = findLeaf(key);
  return (leaf && leaf->key == key) ? &leaf->val : nullptr;
}

template <typename T, typename B>
typename RBLeafTree<T,B>::Node* RBLeafTree<T,B>::findLeaf(const T& key) const {
  Node* x = root;
  while (x && !x->leaf)
    x = (key < x->key) ? x->left : x->right;
  return x;
}

// rot
template <typename T, typename B>
void RBLeafTree<T,B>::leftRotate(Node* x) {
  Node* y = x->right;
  if (!y) return;                    

  // mover subárbol izquierdo de y a x‑right
  x->right = y->left;
  if (y->left)
    y->left->parent = x;

  // enlazar y con padre de x
  y->parent = x->parent;
  if (!x->parent)
    root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;

  // colocar x como hijo izquierdo de y
  y->left = x;
  x->parent = y;
}

template <typename T, typename B>
void RBLeafTree<T,B>::rightRotate(Node* y) {
  Node* x = y->left;
  if (!x) return;

  y->left = x->right;
  if (x->right)
    x->right->parent = y;

  x->parent = y->parent;
  if (!y->parent)
    root = x;
  else if (y == y->parent->left)
    y->parent->left = x;
  else
    y->parent->right = x;

  x->right = y;
  y->parent = x;
}

// insert
template <typename T, typename B>
void RBLeafTree<T,B>::insert(const T& key, const B& val) {
  // caso 0: árbol vacío
  if (!root) {
    root = new Node(key, val, nullptr, nullptr, nullptr, true, BLACK);
    ++sz;
    return;
  }

  // descender hasta la hoja
  std::stack<Node*> ancestors;
  Node* current = root;
  while (!current->leaf) {
    ancestors.push(current);
    current = (key < current->key) ? current->left : current->right;
  }

  // clave ya existente -> actualizar valor 
  if (current->key == key) {
    current->val = val;
    return;
  }

  // dividir la hoja actual en dos hojas rojas 
  Node* oldLeaf = new Node(current->key, current->val, nullptr, nullptr, current, true, RED);
  Node* newLeaf = new Node(key, val, nullptr, nullptr, current, true, RED);

  if (current->key < key) {
    current->left = oldLeaf;
    current->right = newLeaf;
    current->key = key; 
  } else {
    current->left = newLeaf;
    current->right = oldLeaf;
  }

  current->leaf = false;
  current->color = RED; 

  insertFix(current, ancestors);
  ++sz;
}

// reb
template <typename T, typename B>
void RBLeafTree<T,B>::insertFix(Node* z, std::stack<Node*>& anc) {
  while (!anc.empty() && z->parent && z->parent->color == RED) {
    Node* upper = anc.top();
    anc.pop();
    Node* other = (z->parent == upper->left) ? upper->right : upper->left;
    // Caso 1: tío rojo -> cambiamos colores 
    if (other && other->color == RED) {
      z->parent->color = BLACK;
      other->color = BLACK;
      upper->color = RED;
      z = upper;
      continue;
    }
    // Casos 2‑3: tío negro 
    if (z->parent == upper->left) {
      if (z->parent->right && z->parent->right->color == RED) {
        // caso 2.2
        leftRotate(z->parent);
        rightRotate(upper);
        upper->left->color = BLACK;
        upper->right->color = BLACK;
        upper->color = RED;
      } else {
        // caso 2.1
        rightRotate(upper);
        upper->right->color = RED;
      }
    } else { // simétrico 
      if (z->parent->left && z->parent->left->color == RED) {
        // caso 3.2
        rightRotate(z->parent);
        leftRotate(upper);
        upper->right->color = BLACK;
        upper->left->color = BLACK;
        upper->color = RED;
      } else {
        // caso 3.1
        leftRotate(upper);
        upper->left->color = RED;
      }
    }
    break; 
  }
  if (root) root->color = BLACK;
}

// erase
template <typename T, typename B>
bool RBLeafTree<T,B>::erase(const T& key) {
  if (!root) return false;

  std::stack<Node*> path;
  Node* current = root;
  while (!current->leaf) {
    path.push(current);
    current = (key < current->key) ? current->left : current->right;
  }
  if (current->key != key) return false;

  if (current == root) {
    delete root;
    root = nullptr;
    sz = 0;
    return true;
  }

  // eliminar hoja y su padre interno 
  Node* upper = path.top(); path.pop();
  Node* other = (current == upper->left) ? upper->right : upper->left;

  // conectar hermano con el abuelo
  if (other) other->parent = upper->parent;
  if (!upper->parent)
    root = other;
  else if (upper == upper->parent->left)
    upper->parent->left = other;
  else
    upper->parent->right = other;

  bool siblingWasRed = (other && other->color == RED);

  delete current;
  delete upper;
  --sz;

  if (siblingWasRed) {
    other->color = BLACK;   
    return true;
  }

  if (other)
    deleteFix(other);      

  return true;
}

// reb
template <typename T, typename B>
void RBLeafTree<T,B>::deleteFix(Node* current) {
  while (current && current != root && nodeColor(current) == BLACK) {
    Node* parent = current->parent;
    if (!parent) break;

    bool isLeft = (current == parent->left);
    Node* sib   = isLeft ? parent->right : parent->left;

    if (!sib) {
      current = parent;
      continue;
    }

    // Caso 1: hermano rojo 
    if (sib->color == RED) {
      sib->color = BLACK;
      parent->color = RED;
      if (isLeft)  leftRotate(parent);
      else rightRotate(parent);
      sib = isLeft ? parent->right : parent->left;
      if (!sib) {
        current = root;
        break;
      }
    }

    // Caso 2: hermano negro con ambos hijos negros 
    if (nodeColor(sib->left) == BLACK && nodeColor(sib->right) == BLACK) {
      sib->color = RED;
      current = parent; // subir doble‑negro al padre
      continue;
    }

    // Caso 3: hermano negro con hijo cercano rojo y lejano negro 
    if (isLeft && nodeColor(sib->right) == BLACK && nodeColor(sib->left) == RED) {
      sib->left->color = BLACK;
      sib->color = RED;
      rightRotate(sib);
      sib = parent->right;
    }
    else if (!isLeft && nodeColor(sib->left) == BLACK && nodeColor(sib->right) == RED) {
      sib->right->color = BLACK;
      sib->color = RED;
      leftRotate(sib);
      sib = parent->left;
    }

    // Caso 4: hermano negro con hijo lejano rojo 
    sib->color = parent->color;
    parent->color = BLACK;
    if (isLeft) {
      if (sib->right) sib->right->color = BLACK;
      leftRotate(parent);
    } else {
      if (sib->left) sib->left->color = BLACK;
      rightRotate(parent);
    }
    break; 
  }
  if (current) current->color = BLACK;
  if (root) root->color = BLACK;
}

#endif /* RB_LEAF_TREE_H */

