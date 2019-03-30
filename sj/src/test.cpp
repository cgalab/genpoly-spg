#include <iostream> // for endl
#include "edge.h"
#include "point.h"
#include "bst.h"


void printEnum(enum intersect_t val) {
  switch (val) {
    case IS_FALSE:
      std::cerr << "IS_FALSE";
      break;
    case IS_VERTEX:
      std::cerr << "IS_VERTEX";
      break;
    case IS_TRUE:
      std::cerr << "IS_TRUE";
      break;
    case IS_COLLINEAR:
      std::cerr << "IS_COLLINEAR";
      break;
    default:
      break;
  }
}

void test() {
/*
test:  bool checkIntersection(const Edge e1, const Edge e2) 
*/
  std::cerr << "=== checkIntersection tests ===" << std::endl;
  enum intersect_t itest;

  Point p1 = Point(0,0,1,1);
  Point p2 = Point(0,0,2,2);
  Point p3 = Point(0,0,3,3);
  Point p4 = Point(0,0,4,4);

  Edge e1 = Edge(&p1, &p2);
  Edge e2 = Edge(&p3, &p4);

  itest = checkIntersection(e1, e2);
  std::cerr << "intersection should be true : " << ((itest) ? "true" : "false") << " , value: ";
  printEnum(itest);
  std::cerr << std::endl;

  p1.set(1,1);
  p2.set(2,2);
  p3.set(3,3);
  p4.set(4,4);
  itest = checkIntersection(e1, e2);
  std::cerr << "intersection should be false: " << ((itest) ? "true" : "false") << ", value: ";
  printEnum(itest);
  std::cerr << std::endl;

  p1.set(0,0);
  p2.set(0,2);
  p3.set(0,1);
  p4.set(0,3);
  itest = checkIntersection(e1, e2);
  std::cerr << "intersection should be true : " << ((itest) ? "true" : "false") << " , value: ";
  printEnum(itest);
  std::cerr << std::endl;

  p1.set(0,0);
  p2.set(0,2);
  p3.set(0,2);
  p4.set(0,3);
  itest = checkIntersection(e1, e2);
  std::cout << "intersection should be true : " << ((itest) ? "true" : "false") << " , value: ";
  printEnum(itest);
  std::cerr << std::endl;

  p1.set(0,0);
  p2.set(1,1);
  p3.set(1,0);
  p4.set(0,1);
  itest = checkIntersection(e1, e2);
  std::cout << "intersection should be true : " << ((itest) ? "true" : "false") << " , value: ";
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


  std::cerr << "=== Edge class >< comparison tests ===" << std::endl;

  // reuse the points variables from above, p1 & p2 are connected to e1 and p3 & p4 are connected to e2
  p1.set(1,1);
  p2.set(1,2);
  p3.set(1,3);
  p4.set(1,4);

  std::cerr << e1 << " < " << e2 << " should be true : " << ((e1 < e2) ? "true" : "false") << std::endl;
  // check if l_idx changes has an effect..

  // need to check if ebst class is working properly

}