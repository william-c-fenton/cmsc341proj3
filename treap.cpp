// CMSC 341 - Fall 2019 - Project 3
// File: treap.cpp

using namespace std ;

#include "treap.h"

Treap::Treap() {
  _nptr = nullptr ;
}

Treap::Treap(const Treap& other) {
  //
  // Implement copy constructor
  //
  _nptr = nullptr;

  makeCopy(other);
}

Treap::~Treap() {
  //
  // Implement destructor
  //
  //do POST-order traversal (left, right, root) and use remove
  //to delete root.

  makeEmpty();
}

//makeCopy
//assuming an empty base tree, makes a copy from the source tree.
void Treap::makeCopy(const Treap& source){
  if (source._nptr){
    data_t nD = source._nptr->_data;
    priority_t nP = source._nptr->_pri;
    insert(nD, nP);
    makeCopy(source._nptr->_left);
    makeCopy(source._nptr->_right);
  }
}

//deletes the entire tree in post order fashion.
void Treap::makeEmpty(){
  if (_nptr != nullptr){
    _nptr->_left.makeEmpty();
    _nptr->_right.makeEmpty();
    delete _nptr;
    _nptr = nullptr;
  }
}

const Treap& Treap::operator=(const Treap& rhs) {
  //
  // Implement assignment operator
  //
  makeEmpty();
  makeCopy(rhs);
  return *this;
}

// Return -1 if the treap is empty; otherwise, return value in _height.
int Treap::height() const {
  return _nptr ? _nptr->_height : -1 ;
}

// Return the priority of the root node of a Treap.
// Note: numeric_limits<priority_t>::min() returns the minimum value
// of the priority type.  We will only use int priorities, so this is
// the minimum integer value.
priority_t Treap::priority() const {
  return _nptr ? _nptr->_pri : numeric_limits<priority_t>::min() ;
}

const data_t* Treap::find(const data_t& x) {
  //
  // Implement find function
  //
  //recursively go through the tree, left if data value is less than
  //curr node and right if it is more. if data == currnode data
  //then return pointer to the data value.
  data_t *value = nullptr;
  //if you've gotten to an external null node, it's not in the tree.
  if (_nptr == nullptr) return value;

  if (_nptr->_data < x){
    return _nptr->_right.find(x);
  }
  else if (_nptr->_data > x){
    return _nptr->_left.find(x);
  }
  else{
    value = &_nptr->_data;
    return value;
  }

}

//findMin(): Returns pointer to the _nptr of Subtree with lowest data value
TreapNode* Treap::findMin(){
  if(empty()){
    return nullptr;
  }
  else if(_nptr->_left.empty()){
    return _nptr;
  }
  else{
    return _nptr->_left.findMin();
  }
}

// Inorder traversal.  Recommend you NOT change this!!!
void Treap::inorder() {

  if ( empty() ) return ;

  cout << "(" ;
  _nptr->_left.inorder() ;

  cout << _nptr->_data << ":" << priority() << ":" << height() ;

  _nptr->_right.inorder() ;
  cout << ")" ;

  return  ;
}

// Used for debugging and grading.  Interprets the characters in
// position[] as directions in the tree.  E.g., "RLR" means, "starting
// from root, go right, then left, then right."  If position[] is an
// empty string, then it just locats the value a the root.
// See project description for details.
bool Treap::locate (const char position[], data_t& x, priority_t& p, int& h, int offset) {

  if (_nptr == nullptr) return false ;

  if (position[offset] == '\0') {
    x = _nptr->_data ;
    p = _nptr->_pri ;
    h = _nptr->_height ;
    return true ;
  }

  if (position[offset] == 'L') {
    return _nptr->_left.locate(position, x, p, h, offset+1) ;
  }

  if (position[offset] == 'R') {
    return _nptr->_right.locate(position, x, p, h, offset+1) ;
  }

  cerr << "Bad position character!\n" ;
  exit(17) ;

  return false ;
}

//Right rotation:
/* x = _nptr
     x            y
    / \          / \
   y   c  --->  a   x
  / \              / \
 a   b            b   c
 */
void Treap::rightRot(){

  TreapNode *y = _nptr->_left._nptr;
  TreapNode *b;
  if (y != nullptr){
    b = y->_right._nptr;
  }
  else{
    b = nullptr;
  }
  

    //set "x" left equal to b
  _nptr->_left._nptr = b;
  //set "y" right to be equal to "x" 
  y->_right._nptr = _nptr;
  updateHeight();
  _nptr = y;

}

/* Left Rotation:
     x            y
    / \          / \
   a   y  --->  x  c
      / \      / \
      b  c    a  b
*/

void Treap::leftRot(){
  TreapNode *y = _nptr->_right._nptr;
  TreapNode *b;
  if (y != nullptr){
    b = y->_left._nptr;
  }
  else{
    b = nullptr;
  }
  
  
  //set "x" right equal to b
  _nptr->_right._nptr = b;
  //set "y" left to be equal to "x" 
  y->_left._nptr = _nptr;
  updateHeight();
  _nptr = y;
  
  
}

//gets Height of tree using same method that was in the template
void Treap::updateHeight(){
    int leftHeight = _nptr->_left.height(), rightHeight = _nptr->_right.height();
    int height = 1 + ( leftHeight > rightHeight ? leftHeight : rightHeight );
    _nptr->_height = height;
}

// Basic BST insertion.  Does not allow duplicate values.
//
// Complete this function to do Treap insertion.
//
void Treap::insert(const data_t& x, const priority_t& p) {
  if ( empty() ) {
    _nptr = new TreapNode(x, p) ;
  }

  if (_nptr->_data < x) {
    _nptr->_right.insert(x, p) ;

  } else if (x < _nptr->_data ) {
    _nptr->_left.insert(x, p) ;

  }
  
  //if priority is wrong, do rotations to fix
  if(_nptr->_left.priority() > priority()){
    rightRot();
  }
  else if (_nptr->_right.priority() > priority()){
    leftRot();
  }

  // Update height. Maybe this should be a helper function??
  updateHeight();
}

bool Treap::remove(const data_t& x) {
  
  bool flag = false;
  recurRemove(x, flag);
  return flag;

}

//recursive helper for remove().  takes the data value to be removed, 
// and the eventual return value of the remove() function.
void Treap::recurRemove(const data_t& x, bool& flag){
  //navigate to the node to be removed 
  //IF it has only 1 or 0 child:
  //link child of currnode to parent of currnode and delete currnode
  //IF it has 2 child:
  //do rotations until the node can be safely deleted
  TreapNode* curr = _nptr;
  bool test = x < _nptr->_data;

  //navigate to correct node (basic bsc recursive finding)
  if (curr->_data < x) {
   curr->_right.recurRemove(x, flag) ;

  } 
  else if (x < curr->_data ) {
    curr->_left.recurRemove(x, flag) ;

  }
  else if (curr == nullptr){
    return ;
  }

  //update the height after rotations and removal
  if (_nptr != nullptr){
      updateHeight();
    }

  //if curr has one or zero children (same case works) 
  if ((curr->_left.empty() || curr->_right.empty()) && x == curr->_data){
    //link up the parent with the grandchild
    bool hasLeft = curr->_left.empty() ? false : true;
    TreapNode *old = _nptr;
    _nptr = hasLeft ? curr->_left._nptr : curr->_right._nptr;
    
    //cut children off (causes issues with destructor if they are there)
    old->_left._nptr = nullptr;
    old->_right._nptr = nullptr;
    delete old;
    flag = true;
  }
  //if curr has 2 children
  else if ((!curr->_left.empty() && !curr->_right.empty()) && x == curr->_data){

    //if the left priority is lower than the right priority
    if (curr->_left.priority() < curr->_right.priority()){
      //rotate left and see if the case has changed
      leftRot();
      recurRemove(x, flag);
    }
    else{
      rightRot();
      recurRemove(x, flag);
    }

    //this is probably vestigal but I'm too scared to remove it.
    updateHeight();
  }
}

// Treap::dump() just calls TreapNode::dump()
void Treap::dump() {
  if ( !empty() ) _nptr->dump() ;
}

ostream& operator<< (ostream& os, const Treap& T) {
  os << T._nptr ;
  return os ;
}


TreapNode::TreapNode() : _height(0) {
  // Empty constructor body
}

TreapNode::TreapNode(const data_t& x, const priority_t& p)
  : _data(x), _pri(p), _height(0) {
  // Empty constructor body
}

TreapNode::~TreapNode() {
  // Empty destructor
  // Retained for debugging
}

// Dump the contents of a node
void TreapNode::dump() {
  cout << "===== TreapNode::dump()\n" ;
  cout << "      this = " << this << endl ;
  cout << "      left = " << _left << endl ;
  cout << "      right= " << _right << endl ;
  cout << "      data = " << _data << endl ;
  cout << "      height = " << _height << endl ;
  _left.dump() ;
  _right.dump() ;
}

