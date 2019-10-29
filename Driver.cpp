#include <iostream>
#include "treap.h"

using namespace std;

int main() {

  Treap T;

  T.insert("J", 13);
  T.insert("I", 25);
  T.insert("C", 35);
  T.insert("B", 24);
  T.insert("H", 29);
  T.insert("E", 33);
  T.insert("G", 50);
  T.insert("A", 21);
  T.insert("K", 9);
  T.insert("D", 8);
  T.insert("L", 16);

  cout << "prio of root: " << T.priority() << " " << &T << endl;
  T.dump();
  
  cout << endl;
  T.inorder();
  cout << endl;


  return 0;
}
