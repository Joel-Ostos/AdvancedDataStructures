#include <iostream>
#include <string>
#include <ctime>

// #define USE_LEAF_TREE

#include "avl.h"

void testAVLTree() {
  std::cout << "=== Testing AVL Tree Implementation ===" << std::endl;

#ifdef USE_LEAF_TREE
  std::cout << "Using LEAF TREE implementation" << std::endl;
#else
  std::cout << "Using NODE TREE implementation" << std::endl;
#endif

  AVLTree<int, std::string> avl;

  std::cout << "\n1. Inserting elements..." << std::endl;

  // Insertions that would cause imbalance in regular BST
  avl.insert(10, "ten");
  std::cout << "Inserted (10, ten)" << std::endl;
  std::cout << "Tree height: " << avl.getTreeHeight() << ", Balanced: " << (avl.isBalanced() ? "Yes" : "No") << std::endl;

  avl.insert(20, "twenty");
  std::cout << "Inserted (20, twenty)" << std::endl;
  std::cout << "Tree height: " << avl.getTreeHeight() << ", Balanced: " << (avl.isBalanced() ? "Yes" : "No") << std::endl;

  avl.insert(30, "thirty");
  std::cout << "Inserted (30, thirty)" << std::endl;
  std::cout << "Tree height: " << avl.getTreeHeight() << ", Balanced: " << (avl.isBalanced() ? "Yes" : "No") << std::endl;

  avl.insert(40, "forty");
  std::cout << "Inserted (40, forty)" << std::endl;
  std::cout << "Tree height: " << avl.getTreeHeight() << ", Balanced: " << (avl.isBalanced() ? "Yes" : "No") << std::endl;

  avl.insert(50, "fifty");
  std::cout << "Inserted (50, fifty)" << std::endl;
  std::cout << "Tree height: " << avl.getTreeHeight() << ", Balanced: " << (avl.isBalanced() ? "Yes" : "No") << std::endl;

  avl.insert(25, "twenty-five");
  std::cout << "Inserted (25, twenty-five)" << std::endl;
  std::cout << "Tree height: " << avl.getTreeHeight() << ", Balanced: " << (avl.isBalanced() ? "Yes" : "No") << std::endl;

  std::cout << "\n2. Tree traversals:" << std::endl;
  avl.printInorder();
  avl.printPreorder();
  avl.printPostorder();

  std::cout << "\n3. Searching for elements..." << std::endl;
  auto result = avl.find(25);
  if (result != nullptr) {
    std::cout << "Found key 25 with value: " << result->val << std::endl;
  } else {
    std::cout << "Key 25 not found" << std::endl;
  }

  result = avl.find(35);
  if (result != nullptr) {
    std::cout << "Found key 35 with value: " << result->val << std::endl;
  } else {
    std::cout << "Key 35 not found" << std::endl;
  }

  std::cout << "\n4. Deleting elements..." << std::endl;

  avl.deleteNode(40);
  std::cout << "Deleted key 40" << std::endl;
  std::cout << "Tree height: " << avl.getTreeHeight() << ", Balanced: " << (avl.isBalanced() ? "Yes" : "No") << std::endl;
  avl.printInorder();

  avl.deleteNode(25);
  std::cout << "Deleted key 25" << std::endl;
  std::cout << "Tree height: " << avl.getTreeHeight() << ", Balanced: " << (avl.isBalanced() ? "Yes" : "No") << std::endl;
  avl.printInorder();

  avl.deleteNode(20);
  std::cout << "Deleted key 20" << std::endl;
  std::cout << "Tree height: " << avl.getTreeHeight() << ", Balanced: " << (avl.isBalanced() ? "Yes" : "No") << std::endl;
  avl.printInorder();

  std::cout << "\n5. Final tree state:" << std::endl;
  std::cout << "Final tree height: " << avl.getTreeHeight() << std::endl;
  std::cout << "Is balanced: " << (avl.isBalanced() ? "Yes" : "No") << std::endl;
  avl.printInorder();
}

void stressTest() {
  std::cout << "\n=== Stress Test ===" << std::endl;

  AVLTree<int, int> avl;

  // Insert sequential numbers (worst case for regular BST)
  std::cout << "Inserting sequential numbers 1-15..." << std::endl;
  for (int i = 1; i <= 15; i++) {
    avl.insert(i, i * 10);
  }

  std::cout << "Tree height after sequential insertions: " << avl.getTreeHeight() << std::endl;
  std::cout << "Is balanced: " << (avl.isBalanced() ? "Yes" : "No") << std::endl;
  avl.printInorder();

  // Delete some elements
  std::cout << "\nDeleting elements 5, 8, 12..." << std::endl;
  avl.deleteNode(5);
  avl.deleteNode(8);
  avl.deleteNode(12);

  std::cout << "Tree height after deletions: " << avl.getTreeHeight() << std::endl;
  std::cout << "Is balanced: " << (avl.isBalanced() ? "Yes" : "No") << std::endl;
  avl.printInorder();
}

int main() {
  //testAVLTree();
  //stressTest();
  AVLTree<int, int> avl;
  clock_t before = clock();
  for (int i = 0; i < 1e5; i++) {
    avl.insert(i, i);
  }
  clock_t duration = clock() - before;

  std::cout << "Duration: " << (float)duration / CLOCKS_PER_SEC << " seconds";


  return 0;
}
