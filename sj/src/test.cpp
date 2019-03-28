#include <iostream> // for endl
#include "edge.h"
#include "point.h"


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
  enum intersect_t test;

/*
test:  bool checkIntersection(const Edge e1, const Edge e2) 
*/
  std::cerr << "=== checkIntersection tests ===" << std::endl;

  Point p1 = Point(0,0,1,1);
  Point p2 = Point(0,0,2,2);
  Point p3 = Point(0,0,3,3);
  Point p4 = Point(0,0,4,4);

  Edge e1 = Edge(&p1, &p2);
  Edge e2 = Edge(&p3, &p4);

  test = checkIntersection(e1, e2);
  std::cerr << "intersection should be true: " << ((test) ? "true" : "false") << ", value: ";
  printEnum(test);
  std::cerr << std::endl;

  p1.set(1,1);
  p2.set(2,2);
  p3.set(3,3);
  p4.set(4,4);
  test = checkIntersection(e1, e2);
  std::cerr << "intersection should be false: " << ((test) ? "true" : "false") << ", value: ";
  printEnum(test);
  std::cerr << std::endl;

  p1.set(0,0);
  p2.set(0,2);
  p3.set(0,1);
  p4.set(0,3);
  test = checkIntersection(e1, e2);
  std::cerr << "intersection should be true: " << ((test) ? "true" : "false") << ", value: ";
  printEnum(test);
  std::cerr << std::endl;

  p1.set(0,0);
  p2.set(0,2);
  p3.set(0,2);
  p4.set(0,3);
  test = checkIntersection(e1, e2);
  std::cout << "intersection should be true: " << ((test) ? "true" : "false") << ", value: ";
  printEnum(test);
  std::cerr << std::endl;

  p1.set(0,0);
  p2.set(1,1);
  p3.set(1,0);
  p4.set(0,1);
  test = checkIntersection(e1, e2);
  std::cout << "intersection should be true: " << ((test) ? "true" : "false") << ", value: ";
  printEnum(test);
  std::cerr << std::endl;

}