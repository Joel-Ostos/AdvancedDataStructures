#include <cassert>
#include <iostream>
#include "rb_node_tree.h"
int main() {
  RBNodeTree<int,std::string> tree;

  tree.insert(10, "diez");
  tree.insert(5, "cinco");
  tree.insert(20, "veinte");
  assert(tree.size() == 3);

  auto n10 = tree.find(10);
  assert(n10 && n10->val == "diez");
  assert(!tree.find(99));               

  assert(tree.erase(5));            
  assert(tree.size() == 2);
  assert(!tree.find(5));

  for (int k : {1, 30, 15, 7}) tree.insert(k, "x");
  assert(tree.size() == 6);

  assert(tree.erase(20));
  assert(tree.erase(1));
  assert(tree.size() == 4);
  assert(!tree.erase(42));        

  for (int k : {10, 15, 7, 30}) assert(tree.find(k));

  std::cout << "Pruebas básicas superadas.\n";
}

