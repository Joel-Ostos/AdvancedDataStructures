#include <iostream>
#include <set>
#include <string>
#include "leafTree.h"
#include "nodeTree.h"

using namespace std;

int main() {
  leafTree<int, int> leafT;
  nodeTree<int, int> nodeT;
  set<int> s;

  for (int i = 0; i < 1e5; i++) {
    nodeT.insert(i, 1);
  }

  return 0;
}
