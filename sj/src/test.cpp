#include <iostream> // for endl
#include <set>
#include "edge.h"
#include "point.h"
//#include "bst.h"
#include "basicFunctions.h" //for setComp (Edge set test)


void printEnum(enum intersect_t val) {
  switch (val) {
    case IS_FALSE:
      std::cerr << "IS_FALSE";
      break;
    case IS_VERTEX11:
      std::cerr << "IS_VERTEX11";
      break;
    case IS_VERTEX12:
      std::cerr << "IS_VERTEX12";
      break;
    case IS_VERTEX21:
      std::cerr << "IS_VERTEX21";
      break;
    case IS_VERTEX22:
      std::cerr << "IS_VERTEX22";
      break;
    case IS_TRUE:
      std::cerr << "IS_TRUE";
      break;
    case IS_COLLINEAR:
      std::cerr << "IS_COLLINEAR";
      break;
    case IS_SAME_EDGE:
      std::cerr << "IS_SAME_EDGE";
    default:
      break;
  }
}
/*
void printEnum(enum bst_t val) {
  switch (val) {
    case BST_SUCCESS:
      std::cerr << "BST_SUCCESS";
      break;
    case BST_INTERSECTION:
      std::cerr << "BST_INTERSECTION";
      break;
    case BST_EXISTS:
      std::cerr << "BST_EXISTS";
      break;
    case BST_UNDEFINED:
      std::cerr << "BST_UNDEFINED";
      break;
    default:
      break;
  }
}
*/

void test() {
/*
test:  bool checkIntersection(const Edge e1, const Edge e2)
*/
  std::cerr << "=== checkIntersection tests ===" << std::endl;
  std::cerr << "=== 'should be true' is a comparison 'itest >= IS_TRUE' : true when intersection exists ===" << std::endl;
  enum intersect_t itest;

  Point p1 = Point(0,0,1,1);
  Point p2 = Point(0,0,2,2);
  Point p3 = Point(0,0,3,3);
  Point p4 = Point(0,0,4,4);

  Edge e1 = Edge(&p1, &p2);
  Edge e2 = Edge(&p3, &p4);

  itest = checkIntersection(e1, e2);
  std::cerr << "intersection: " << e1 << " with " << e2 << ", should be true : " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";
  printEnum(itest);
  std::cerr << std::endl;

  p1.set(1,1);
  p2.set(2,2);
  e1.set(p1,p2);
  p3.set(1,1);
  p4.set(2,2);
  e2.set(p3,p4);
  itest = checkIntersection(e1, e2);
  std::cerr << "intersection: " << e1 << " with " << e2 << ", should be true : " << ((itest >= IS_TRUE) ? "true" : "false") << ", value: ";
  printEnum(itest);
  std::cerr << std::endl;

  p1.set(1,1);
  p2.set(2,2);
  e1.set(p1,p2);
  p3.set(3,3);
  p4.set(4,4);
  e2.set(p3,p4);
  itest = checkIntersection(e1, e2);
  std::cerr << "intersection: " << e1 << " with " << e2 << ", should be false: " << ((itest >= IS_TRUE) ? "true" : "false") << ", value: ";
  printEnum(itest);
  std::cerr << std::endl;

  p1.set(0,0);
  p2.set(0,2);
  e1.set(p1,p2);
  p3.set(0,1);
  p4.set(0,3);
  e2.set(p3,p4);
  itest = checkIntersection(e1, e2);
  std::cerr << "intersection: " << e1 << " with " << e2 << ", should be true : " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";
  printEnum(itest);
  std::cerr << std::endl;

  p1.set(0,0);
  p2.set(0,2);
  e1.set(p1,p2);
  p3.set(0,2);
  p4.set(0,3);
  e2.set(p3,p4);
  itest = checkIntersection(e1, e2);
  std::cout << "intersection: " << e1 << " with " << e2 << ", should be false: " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";
  printEnum(itest);
  std::cerr << std::endl;

  p1.set(0,0);
  p2.set(1,1);
  e1.set(p1,p2);
  p3.set(1,0);
  p4.set(0,1);
  e2.set(p3,p4);
  itest = checkIntersection(e1, e2);
  std::cout << "intersection: " << e1 << " with " << e2 << ", should be true : " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";
  printEnum(itest);
  std::cerr << std::endl;

  p1.set(0.2,0);
  p2.set(0.2,0.2);
  e1.set(p1,p2);
  p3.set(0.1,0.1);
  p4.set(0.2,0);
  e2.set(p3,p4);
  itest = checkIntersection(e1, e2);
  std::cout << "intersection: " << e1 << " with " << e2 << ", should be false : " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";
  printEnum(itest);
  std::cerr << std::endl;

  p1.set(0.3,0.5);
  p2.set(0.8,0.9);
  e1.set(p1,p2);
  p3.set(0,0.4);
  p4.set(0.8,0.9);
  e2.set(p3,p4);
  itest = checkIntersection(e1, e2);
  std::cout << "intersection: " << e1 << " with " << e2 << ", should be false : " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";
  printEnum(itest);
  std::cerr << std::endl;

  std::cerr << std:: endl;
/*
  Testing Yval class in edge.h
*/
  std::cerr << "=== Yval tests ===" << std::endl;

  Yval y1 = Yval(1,2);
  Yval y2 = Yval(3,4);

  std::cerr << y1 << " < " << y2 << " should be true : " << ((y1 < y2) ? "true" : "false") << std::endl;

  y2.set(2,3);
  std::cerr << y1 << " < " << y2 << " should be true : " << ((y1 < y2) ? "true" : "false") << std::endl;

  y2.set(1.5,3);
  std::cerr << y1 << " < " << y2 << " should be true : " << ((y1 < y2) ? "true" : "false") << std::endl;

  y2.set(1,3);
  std::cerr << y1 << " < " << y2 << " should be true : " << ((y1 < y2) ? "true" : "false") << std::endl;

  y2.set(1,2);
  std::cerr << y1 << " < " << y2 << " should be false : " << ((y1 < y2) ? "true" : "false") << std::endl;

  y2.set(3);
  std::cerr << y1 << " < " << y2 << " should be true : " << ((y1 < y2) ? "true" : "false") << std::endl;

  y2.set(2);
  std::cerr << y1 << " < " << y2 << " should be true : " << ((y1 < y2) ? "true" : "false") << std::endl;

  y2.set(1.1);
  std::cerr << y1 << " < " << y2 << " should be true : " << ((y1 < y2) ? "true" : "false") << std::endl;

  y2.set(1);
  std::cerr << y1 << " < " << y2 << " should be false : " << ((y1 < y2) ? "true" : "false") << std::endl;

  std::cerr << std:: endl;
/*
  std::cerr << "=== Edge class >< comparison tests ===" << std::endl;

  // reuse the points variables from above, p1 & p2 are connected to e1 and p3 & p4 are connected to e2
  p1.set(1,1);
  p2.set(1,2);
  p3.set(1,3);
  p4.set(1,4);
  e1.l_idx = 1;
  e2.l_idx = 1;

  std::cerr << e1 << " < " << e2 << " should be true : " << ((e1 < e2) ? "true" : "false") << std::endl;

  // check if l_idx changes has an effect.
  p1.set(0,0);
  p2.set(1,1);
  p3.set(0,1);
  p4.set(1,0);
  e1.l_idx = 0;
  e2.l_idx = 0;

  std::cerr << e1 << " < " << e2 << " should be true : " << ((e1 < e2) ? "true" : "false") << std::endl;
  e1.l_idx = 1;
  std::cerr << e1 << " < " << e2 << " should be false: " << ((e1 < e2) ? "true" : "false") << std::endl;

  p1.set(1,0);
  e1.l_idx = 1;

  std::cerr << e1 << " < " << e2 << " should be false: " << ((e1 < e2) ? "true" : "false") << std::endl;

  // testing if an interval on y axis: [0,1] at x coord: 1 is < of a point at (1,1)
  p4.set(1,1);
  std::cerr << e1 << " < " << e2 << " should be true : " << ((e1 < e2) ? "true" : "false") << std::endl;

  //moving the interval up 0.5
  p1.set(1,0.5);
  p2.set(1,1.5);
  std::cerr << e1 << " < " << e2 << " should be true : " << ((e1 < e2) ? "true" : "false") << std::endl;
  // it is true because an interval at e1's l_idx uses its min value.

  // 2 interval comparison at x coord: 1
  p1.set(1,1);
  p2.set(1,2);
  p3.set(1,3);
  p4.set(1,4);
  e1.l_idx = 1;
  e2.l_idx = 1;

  std::cerr << e1 << " < " << e2 << " should be true : " << ((e1 < e2) ? "true" : "false") << std::endl;

  p2.set(1,3);
  std::cerr << e1 << " < " << e2 << " should be true : " << ((e1 < e2) ? "true" : "false") << std::endl;

  p3.set(1,1.1);
  std::cerr << e1 << " < " << e2 << " should be true : " << ((e1 < e2) ? "true" : "false") << std::endl;

  p1.set(1,1);
  p2.set(1,3);
  p3.set(1,1);
  p4.set(1,3);
  std::cerr << e1 << " < " << e2 << " should be false: " << ((e1 < e2) ? "true" : "false") << std::endl;

  p4.set(1,2.8);
  std::cerr << e1 << " < " << e2 << " should be false: " << ((e1 < e2) ? "true" : "false") << std::endl;

  std::cerr << std:: endl;
*/
  std::cerr << "=== Edge set tests ===" << std::endl;

  double d_idx=0;
  std::set<Edge, setComp> edgeS(d_idx); // a set of an edge with 'setComp' as a '<' comparison function.
  std::pair<std::set<Edge, setComp>::iterator,bool> retval; // return value

  p1.set(0,0);
  p2.set(3,3);
  e1.set(p1,p2);
  p3.set(1,0);
  p4.set(1,3);
  e2.set(p3,p4);
  std::cerr << "inserting e1:" << e1 << std::endl;
  retval = edgeS.insert(e1);
  std::cerr << "r.1: " << (*retval.first) << std::endl;
  std::cerr << "r.2 should be true for a correct insertion: " << (retval.second ? "true" : "false") << std::endl;
  std::cerr << "inserting e2:" << e2 << std::endl;
  d_idx = 1;
  retval = edgeS.insert(e2);
  std::cerr << "r.1: " << (*retval.first) << std::endl;
  std::cerr << "r.2 should be true for a correct insertion: " << (retval.second ? "true" : "false") << std::endl;
  std::cerr << "set.begin(): " << (*edgeS.begin()) << std::endl;
  std::cerr << "set.end()  : " << *(--edgeS.end()) << std::endl;

  Point p5 = Point(2,0,5,5);
  Point p6 = Point(2,3,6,6);

  Edge e3 = Edge(&p5, &p6);
  d_idx = 2;

  std::cerr << "inserting e3:" << e3 << std::endl;
  retval = edgeS.insert(e3);
  std::cerr << "r.1: " << (*retval.first) << std::endl;
  std::cerr << "r.2 should be true for a correct insertion: " << (retval.second ? "true" : "false") << std::endl;
  std::cerr << "set.begin(): " << (*edgeS.begin()) << std::endl;
  std::cerr << "set.end()  : " << *(--edgeS.end()) << std::endl;


/*
  // need to check if ebst class is working properly
    std::cerr << "=== Edge Binary Search Tree class tests ===" << std::endl;

  ebst tree;
  std::pair<BSTNode*, enum bst_t> retval;

  p1.set(0,0);
  p2.set(3,3);
  e1.l_idx = 0;
  p3.set(1,0);
  p4.set(1,3);
  e2.l_idx = 1;
  std::cout << tree << std::endl;

  retval = tree.insert(e1);
  std::cout << "e1 enum: ";
  printEnum(retval.second);
  std::cout << std::endl;
  std::cout << tree << std::endl;

  retval = tree.insert(e2);
  std::cout << "e2 enum: ";
  printEnum(retval.second);
  std::cout << std::endl;
  std::cout << tree << std::endl;
*/
}
