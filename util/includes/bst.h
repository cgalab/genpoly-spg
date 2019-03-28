#include <utility> // for std::pair

// Header for binary search tree definitions for edges
#ifndef __BST_H_
#define __BST_H_

enum class bst_t {
  BST_SUCCESS,
  BST_INTERSECTION,
  BST_EXISTS,
  BST_UNDEFINED
};

class BSTNode {
public:
  Edge key;
  BSTNode * left;
  BSTNode * right;
  BSTNode * parent;
  int balance;

  BSTNode() {key = new Edge(); left=NULL;right=NULL;parent=NULL;balance=0}
  BSTNode(BSTNODE* prnt) {key=new Edge(); left=NULL;right=NULL;parent=prnt;balance=0;}
  BSTNode(Edge val, prnt) {key=val;left=NULL;right=NULL;parent=prnt;balance=0;}

  void rotateLeft() {
    // A -> B -> C ==> A <- B -> C
    // where 'this' node is A
    BSTNode* root = this->parent;
    this->parent = this->right;    // B parent of A
    this->right = NULL;

    (*this->parent).left = this;  // A left child of B
    (*this->parent).parent = root;// parent node moved from A to B

    this->balance = 0;
    (this->parent).balance = 0;
  }
  void rotateRight() {
    // A <- B <- C ==> A <- B -> C
    // Where 'this' node is C
    BSTNode* root = this->parent;
    this->parent = this->left;    // B parent of C
    this->left = NULL;

    (*this->parent).right = this; // C right child of B
    (*this->parent).parent = root;// parent node moved from C to B

    this->balance = 0;
    (this->parent).balance = 0;
  }
  void rotateLeftRight() {
    //  A <- C; A -> B ==> A <- B -> C
    // Where 'this' node is C
    BSTNode* root = this->parent;
    BSTNODE* A = this->left;
    this->left = NULL;
    (*A).parent = (*A).right; // A <- B
    (*A).right = NULL;
    this->parent = (*A).parent; // B -> C
    (*this->parent).right = this; // C right child of B
    (*this->parent).left = A; // A left child of B
    (*this->parent).parent = root;

    if ( (*this->parent).balance > 0 ) { // if B was right-heavy
      this->balance = 0;
      (*A).balance = -1;
    }
    else if ( (*this->parent).balance == 0 ) { // if B was even
      this->balance = 0;
      (*A).balance = 0;
    }
    else {
      this->balance = 1;
      (*A).balance = 0;
    }

    (*this->parent).balance = 0;
  }
  void rotateRightLeft() {
    // A -> C; B <- C ==> A <- B -> C
    // Where 'this' node is A
    BSTNode* root = this->parent;
    BSTNODE* C = this->right;
    this->right = NULL;
    (*C).parent = (*C).left; // B -> C
    (*C).left = NULL;
    this->parent = (*C).parent; // A <- B
    (*this->parent).left = this; // A left child of B
    (*this->parent).right = C; // C right child of B
    (*this->parent).parent = root;

    if ( (*this->parent).balance > 0 ) { // if B was right-heavy
      this->balance = -1;
      (*C).balance = 0;
    }
    else if ( (*this->parent).balance == 0 ) { // if B was even
      this->balance = 0;
      (*C).balance = 0;
    }
    else {
      this->balance = 0;
      (*C).balance = 1;
    }

    (*this->parent).balance = 0;
  }

  std::pair<BSTNODE*, enum bst_t> insert(Edge val) {
    std::pair<BSTNODE*, enum bst_t> response;

    if (key == val) {
      response = std::make_pair(this, BST_EXISTS);
      return response;
    }

    if (key.p1 == NULL) {
      key.set(val);
      response = std::make_pair(this, BST_SUCCESS);
      return response;
    }

    // An edge can not be added if it intersects another edge
    if (checkIntersection(key, val) < IS_TRUE) {
      // no intersection
      if (val < key) {
        if (left == NULL) left = new BSTNode(this);
        response = left.insert(val);
        if (response.second == BST_SUCCESS) --balance;
      }
      else {
        if (right == NULL) right = new BSTNode(this);
        response = right.insert(val);
        if (response.second == BST_SUCCESS) ++balance;
      }
    } else {
      // there is an intersection
      response = std::make_pair(this, BST_INTERSECTION);
      return response;
    }

    if (balance < -1) {
      // too left-heavy, need to rotate 
      if ((*left).balance > 0) this->rotateLeftRight();
      else this->rotateRight();
    }

    if (balance > 1) {
      // too right-heavy need to rotate
      if ((*right).balance < 0) this->rotateRightLeft();
      else this->rotateLeft();
    }

    return response;
  }

  std::pair<BSTNODE*, enum bst_t> del(Edge val) {
    if (val < key) (*left).del(val);
    if (val > key) (*right).del(val);

    if (left && right) {

    }
    else if (left) {

    }
    else if (right) {

    }
    else {

    }
  }

};

template <class T>
class bst {
private:
  BSTNode * root;
  long int size;
public:
  bst() { root = NULL; size = 0;}

  std::pair<BSTNODE*, enum bst_t> insert(T val) {
    std::pair<BSTNODE*, enum bst_t> response;
    if (root==NULL) {
      root = new BSTNode(val);
    }
    
    response = root.insert(val);
    if (response.second == BST_SUCCESS) ++size;
    return response; 
  }
  find(T val) const {} 
  lower_bound (T val) const {}
  upper_bound (t val) const {}

}

#endif