#include <utility> // for std::pair
#include "basicDefinitions.h"

// Header for binary search tree definitions for edges
#ifndef __BST_H_
#define __BST_H_

enum bst_t {
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

  BSTNode() {key = Edge(); left=NULL;right=NULL;parent=NULL;balance=0;}
  BSTNode(BSTNode * PARENT) {key= Edge(); left=NULL;right=NULL;parent=PARENT;balance=0;}
  BSTNode(Edge val, BSTNode* PARENT) {key=val;left=NULL;right=NULL;parent=PARENT;balance=0;}

  void rotateLeft() {
    // A -> B  ==> A <- B
    // where 'this' node is A
    BSTNode* root = this->parent;
    this->parent = this->right;    // B parent of A
    this->right = NULL;

    (*this->parent).left = this;  // A left child of B
    (*this->parent).parent = root;// parent node moved from A to B

    if (this->parent == NULL) { // if B was 0, and we rotated because of deletion // NEED TO FIX...  DELETION DOESN'T WORK THIS WAY
      this->balance = 1;
      (*this->parent).balance = -1;
    } else {
      this->balance = 0;
      (*this->parent).balance = 0;
    }
  }
  void rotateRight() {
    // B <- C ==> B -> C
    // Where 'this' node is C
    BSTNode* root = this->parent;
    this->parent = this->left;    // B parent of C
    this->left = NULL;

    (*this->parent).right = this; // C right child of B
    (*this->parent).parent = root;// parent node moved from C to B

    if (this->parent == NULL) {   // if 'balance' for B was 0, and we rotated because of deletion // NEED TO FIX... 
      this->balance = -1;
      (*this->parent).balance = 1;
    } else {
      this->balance = 0;
      (*this->parent).balance = 0;
    }
  }
  void rotateLeftRight() {
    //  A <- C; A -> B ==> A <- B -> C
    // Where 'this' node is C
    BSTNode* root = this->parent;
    BSTNode* A = this->left;
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
    BSTNode* C = this->right;
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

  BSTNode* find_succ() {
    BSTNode* next = right;
    while(next->left != NULL) {
      next = next->left;
    }
    return next;
  }
  BSTNode* find_pred() {
    BSTNode* next = left;
    while(next->right != NULL) {
      next = next->right;
    }
    return next;
  }

  /* function to insert a node into a balanced binary tree. */
  std::pair<BSTNode*, enum bst_t> insert(Edge val) {
    std::pair<BSTNode*, enum bst_t> response;

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
        response = left->insert(val);
        if (response.second == BST_SUCCESS) --balance;
      }
      else {
        if (right == NULL) right = new BSTNode(this);
        response = right->insert(val);
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

  /* Function to delete a BSTNode in a balanced binary tree */
  std::pair<BSTNode*, enum bst_t> del(Edge val) {
    std::pair<BSTNode*, enum bst_t> response;
    if (val < key) (*left).del(val);
    if (val > key) (*right).del(val);

    if (left && right) {
      if (balance < 0) {
        // move the in-order predecessor to this node, if it had a child, move the child to the predecessors old placement.
        BSTNode* pred = this->find_pred();
        this->key = pred->key;
        if (pred->left != NULL) { // have to connect the right child to the parent and vice versa.
          (*pred->parent).right = pred->left;
          (*pred->left).parent = pred->parent;
        }
        delete pred;
      } else {
        // move the in-order successor to this node, if it had a child, move the child to the successors old placement.
        BSTNode* succ = this->find_succ();
        this->key = succ->key;
        if (succ->right != NULL) { // have to connect the right child to the parent and vice versa.
          (*succ->parent).left = succ->right;
          (*succ->right).parent = succ->parent;
        }
        delete succ;
      }
      response = std::make_pair(this, BST_SUCCESS);
    }
    else if (left) { // the left child replaces 'this' as a child, and 'this's parent becomes the left child's parent.
      if(this == (*this->parent).left) // if we replaced the parent's left child
        (*this->parent).left = this->left;
      else 
        (*this->parent).right = this->left;
      (*this->left).parent = this->parent;
      response = std::make_pair(this->left, BST_SUCCESS);
      delete this;
    }
    else if (right) {
      if(this == (*this->parent).left)
        (*this->parent).left = this->right;
      else
        (*this->parent).right = this->right;
      (*this->right).parent = this->parent;
      response = std::make_pair(this->right, BST_SUCCESS);
      delete this;
    }
    else { // no children.
      if(this == (*this->parent).left)
        (*this->parent).left = NULL;
      else
        (*this->parent).right = NULL;
      response = std::make_pair(this->parent, BST_SUCCESS);
      delete this;
    }
    return response;
  }

};

class ebst {
private:
  BSTNode * root;
  long int size;
public:
  ebst() {root = NULL; size = 0;}

  std::pair<BSTNode*, enum bst_t> insert(Edge val) {
    std::pair<BSTNode*, enum bst_t> response;
    if (root==NULL) {
      root = new BSTNode(val, NULL);
    }
    
    response = root->insert(val);
    if (response.second == BST_SUCCESS) ++size;
    return response;
  }

  std::pair<BSTNode*, enum bst_t> del(Edge val) {
    std::pair<BSTNode*, enum bst_t> response;
    response = root->del(val);
    return response;
  }
  //find(Edge val) const {}
  //lower_bound (Edge val) const {}
  //upper_bound (Edge val) const {}
};

#endif