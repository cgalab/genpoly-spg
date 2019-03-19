#include <iostream> // for endl
#include "edge.h"
#include "point.h"


void test() {
  bool test = false;

/*
test:  bool checkIntersection(const Edge e1, const Edge e2) 
*/

  Point p1 = Point(0,0,1,1);
  Point p2 = Point(0,0,2,2);
  Point p3 = Point(0,0,3,3);
  Point p4 = Point(0,0,4,4);

  Edge e1 = Edge(&p1, &p2);
  Edge e2 = Edge(&p3, &p4);

  test = checkIntersection(e1, e2);
  std::cout << "intersection should be true: " << ((test) ? "true" : "false") << std::endl;

  p1.set(1,1);
  p2.set(2,2);
  p3.set(3,3);
  p4.set(4,4);
  test = checkIntersection(e1, e2);
  std::cout << "intersection should be false: " << ((test) ? "true" : "false") << std::endl;

  p1.set(0,0);
  p2.set(0,2);
  p3.set(0,1);
  p4.set(0,3);
  test = checkIntersection(e1, e2);
  std::cout << "intersection should be true: " << ((test) ? "true" : "false") << std::endl;

  p1.set(0,0);
  p2.set(0,2);
  p3.set(0,2);
  p4.set(0,3);
  test = checkIntersection(e1, e2);
  std::cout << "intersection should be true: " << ((test) ? "true" : "false") << std::endl;

  p1.set(0,0);
  p2.set(1,1);
  p3.set(1,0);
  p4.set(0,1);
  test = checkIntersection(e1, e2);
  std::cout << "intersection should be true: " << ((test) ? "true" : "false") << std::endl;

}