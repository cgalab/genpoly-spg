#include <iostream> // for endl
#include <set>
#include <math.h>
#include <iomanip>      // std::setprecision
#include "edge.h"
#include "point.h"
#include "opt2.h"
//#include "bst.h"
#include "basicFunctions.h" //for setComp (Edge set test)
#include "pol.h"
#include "heap.h"
#include "predicates.h"


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
  Point p1 = Point(0,0,0,0);
  p1.l = 1;
  Point p2 = Point(0,0,1,1);
  p2.l = 2;
  Point p3 = Point(0,0,2,2);
  p3.l = 3;
  Point p4 = Point(0,0,3,3);
  p4.l = 4;

  std::cerr << "=== Point comparison tests ===" << std::endl;

  p1.set(1,1);
  p2.set(2,1);
  p3.set(1,2);

  std::cerr << p1 << "  < " << p1 << ", should be false: " << ((p1 < p1) ? "true" : "false") << std::endl;
  std::cerr << p1 << "  > " << p1 << ", should be false: " << ((p1 > p1) ? "true" : "false") << std::endl;
  std::cerr << p1 << " == " << p1 << ", should be true : " << ((p1 == p1) ? "true" : "false") << std::endl;
  std::cerr << p1 << " != " << p1 << ", should be false: " << ((p1 != p1) ? "true" : "false") << std::endl;
  std::cerr << p1 << "  < " << p2 << ", should be true : " << ((p1 < p2) ? "true" : "false") << std::endl;
  std::cerr << p1 << "  > " << p2 << ", should be false: " << ((p1 > p2) ? "true" : "false") << std::endl;
  std::cerr << p1 << " == " << p2 << ", should be false: " << ((p1 == p2) ? "true" : "false") << std::endl;
  std::cerr << p1 << " != " << p2 << ", should be true : " << ((p1 != p2) ? "true" : "false") << std::endl;
  std::cerr << p1 << "  < " << p3 << ", should be true : " << ((p1 < p3) ? "true" : "false") << std::endl;
  std::cerr << p1 << "  > " << p3 << ", should be false: " << ((p1 > p3) ? "true" : "false") << std::endl;
  std::cerr << p1 << " == " << p3 << ", should be false: " << ((p1 == p3) ? "true" : "false") << std::endl;
  std::cerr << p1 << " != " << p3 << ", should be true : " << ((p1 != p3) ? "true" : "false") << std::endl;

  std::cerr << std::endl;

  std::cerr << "=== Edge comparison with points ===" << std::endl;

  p1.set(118532,919572,32577,48380,32577);
  p2.set(118532,978866,32578,48379,32578);
  p3.set(2,2,2,2,2);
  p4.set(3,3,3,3,3);

  Edge e1 = Edge(&p1, &p2);
  Edge e2 = Edge(&p3, &p4);

  std::cerr << "(*e1.p1): " << *e1.p1 << " == *p1: " << p1 << " : " <<  ((*e1.p1 == p1) ? "true" : "false") << std::endl;
  std::cerr << "(*e1.p2): " << *e1.p1 << " == *p2: " << p2 << " : " <<  ((*e1.p2 == p2) ? "true" : "false") << std::endl;
  std::cerr << std::endl;

  p1.set(0.1776512115938656,0.59801348903157514,3008,2320,3010);
  p2.set(0.32086761784458201,0.58090142848487081,3415,2321,3415);
  p3.set(0.17057308659368181,0.59801348903163565,2985,2317,2985);
  p4.set(0.1776512115938656,0.59801348903157514,3008,2320,3010);
  e1.set(p1, p2);
  e2.set(p3, p4);

  std::cerr << std::setprecision(15);
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;
  std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;
  std::cerr << "e2 < e1: " << ((e2 < e1) ? "true" : "false") << std::endl;


  std::cerr << "=== checkIntersection tests ===" << std::endl;
  std::cerr << "=== 'should be true' is a comparison 'itest >= IS_TRUE' : true when intersection exists ===" << std::endl;
  enum intersect_t itest;

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

  p1.set(0.2,0.2);
  p2.set(0.3,0.5);
  e1.set(p1,p2);
  p3.set(0.2,0.2);
  p4.set(1,0);
  e2.set(p3,p4);
  itest = checkIntersection(e1, e2);
  std::cout << "intersection: " << e1 << " with " << e2 << ", should be false : " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";
  printEnum(itest);
  std::cerr << std::endl;

  p1.set(0.2,0);
  p2.set(0.6,0);
  e1.set(p1,p2);
  p3.set(0.2,0);
  p4.set(1,0);
  e2.set(p3,p4);
  itest = checkIntersection(e1, e2);
  std::cout << "intersection: " << e1 << " with " << e2 << ", should be false: " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";
  printEnum(itest);
  std::cerr << std::endl;

  p1.set(0.2,0);
  p2.set(0.2,0.2);
  e1.set(p1,p2);
  p3.set(0.1,0.1);
  p4.set(0.9,0.9);
  e2.set(p3,p4);
  itest = checkIntersection(e1, e2);
  std::cout << "intersection: " << e1 << " with " << e2 << ", should be true : " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";
  printEnum(itest);
  std::cerr << std::endl;

  p1.set(0.2,0);
  p2.set(0.3,0.5);
  e1.set(p1,p2);
  p3.set(0.2,0.2);
  p4.set(0.3,0.5);
  e2.set(p3,p4);
  itest = checkIntersection(e1, e2);
  std::cout << "intersection: " << e1 << " with " << e2 << ", should be false: " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";
  printEnum(itest);
  std::cerr << std::endl;

  p1.set(2,15);
  p2.set(2,17);
  e1.set(p1,p2);
  p3.set(2,14);
  p4.set(2,17);
  e2.set(p3,p4);
  itest = checkIntersection(e1, e2);
  std::cout << "intersection: " << e1 << " with " << e2 << ", should be true: " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";
  printEnum(itest);
  std::cerr << std::endl;

  p1.set(4,4);
  p2.set(4,7);
  e1.set(p1,p2);
  p3.set(4,6);
  p4.set(4,7);
  e2.set(p3,p4);

  itest = checkIntersection(e1, e2);
  std::cout << "intersection: " << e1 << " with " << e2 << ", should be true: " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";
  printEnum(itest);
  std::cerr << std::endl;

  p1.set(12982,6568);
  p2.set(13026,6584);
  e1.set(p1,p2);
  p3.set(12410,6360);
  p4.set(13026,6584);
  e2.set(p3,p4);

  itest = checkIntersection(e1, e2);
  std::cout << "intersection: " << e1 << " with " << e2 << ", should be true: " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";
  printEnum(itest);
  std::cerr << std::endl;

  p1.set(12312,4738);
  p2.set(12314,4734);
  e1.set(p1,p2);
  p3.set(12308,4746);
  p4.set(12314,4734);
  e2.set(p3,p4);

  itest = checkIntersection(e1, e2);
  std::cout << "intersection: " << e1 << " with " << e2 << ", should be true: " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";
  printEnum(itest);
  std::cerr << std::endl;

  p1.set(-0.6034851544931661, -0.0572553943219186);
  p2.set(-0.6034827734680771, -0.0568532372913607);
  e1.set(p1,p2);
  p3.set(-0.6034851421330270, -0.0572554178266569);
  p4.set(-0.6034829978440949, -0.0568532654878436);
  e2.set(p3,p4);

  itest = checkIntersection(e1, e2);
  std::cout << "intersection: " << e1 << " with " << e2 << ", should be false: " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";
  printEnum(itest);
  std::cerr << std::endl;

  p1.set(-0.0003922779898438,  0.1603560581152343);
  p2.set(-0.0002011321564974,  0.2376256472348480);
  e1.set(p1,p2);
  p3.set(-0.0004140803492565,  0.1464613835869469);
  p4.set(-0.0002011321556713,   0.2376256472341352);
  e2.set(p3,p4);

  itest = checkIntersection(e1, e2);
  std::cout << "intersection: " << e1 << " with " << e2 << ", should be false: " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";
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
  std::cerr << y2 << " < " << y1 << " should be true : " << ((y2 < y1) ? "true" : "false") << std::endl;

  y1.set(10);
  y1.setX(7);
  y2.set(15,16);
  y2.setX(6);
  std::cerr << y1 << " < " << y2 << " should be false : " << ((y1 < y2) ? "true" : "false") << std::endl;

  y1.set(4342);
  y1.setX(4972);
  y2.set(4342);
  y2.setX(4972);
  std::cerr << y1 << " == " << y2 << " should be true : " << ((y1 == y2) ? "true" : "false") << std::endl;

  p1.set(49720,4342);
  p2.set(76260,670);
  p3.set(49720,4342);
  p4.set(66520,4596);
  y1 = getYatX(e1, 49720);
  y2 = getYatX(e2, 49720);
  std::cerr << std::setprecision(15) << y1 << " == " << y2 << " should be true : " << ((y1 == y2) ? "true" : "false") << std::endl;

  std::cerr << std:: endl;

  std::cerr << "=== isPol1Left test ===" << std::endl;

  p1.setV(29);
  p2.setV(0);
  std::cerr << "is p1 1 left of p2 in pol; should be true: " << ((isPol1Left(&p1, &p2, 30) ? "true" : "false")) << std::endl;
  p1.setV(0);
  p2.setV(29);
  std::cerr << "is p1 1 left of p2 in pol; should be false: " << ((isPol1Left(&p1, &p2, 30) ? "true" : "false")) << std::endl;
  p1.setV(0);
  p2.setV(1);
  std::cerr << "is p1 1 left of p2 in pol; should be true: " << ((isPol1Left(&p1, &p2, 30) ? "true" : "false")) << std::endl;
  p1.setV(1);
  p2.setV(0);
  std::cerr << "is p1 1 left of p2 in pol; should be false: " << ((isPol1Left(&p1, &p2, 30) ? "true" : "false")) << std::endl;
  p1.setV(5);
  p2.setV(6);
  std::cerr << "is p1 1 left of p2 in pol; should be true: " << ((isPol1Left(&p1, &p2, 30) ? "true" : "false")) << std::endl;
  p1.setV(5);
  p2.setV(7);
  std::cerr << "is p1 1 left of p2 in pol; should be false: " << ((isPol1Left(&p1, &p2, 30) ? "true" : "false")) << std::endl;

  std::cerr << std:: endl;

  std::cerr << "=== Edge class >< comparison tests ===" << std::endl;

  // reuse the points variables from above, p1 & p2 are connected to e1 and p3 & p4 are connected to e2
  p1.set(1,1);
  p2.set(1,2);
  p3.set(1,1);
  p4.set(1,2);
  e1.l_idx = 1;
  e2.l_idx = 1;

  std::cerr << e1 << " == " << e2 << " should be true : " << ((e1 == e2) ? "true" : "false") << std::endl;

  std::cerr << std::endl;
  std::cerr << "=== Edge set tests ===" << std::endl;

  std::set<Edge> edgeS; // a set of edges
  std::pair<std::set<Edge>::iterator,bool> retval; // return value

  p1.set(0,0.6);
  p2.set(1,0);
  e1.set(p1,p2);
  p3.set(0,0.6);
  p4.set(0.8,0.9);
  e2.set(p3,p4);
  std::cerr << "inserting e1:" << e1 << std::endl;
  retval = edgeS.insert(e1);
  std::cerr << "r.1: " << (*retval.first) << std::endl;
  std::cerr << "r.2 should be true for a correct insertion: " << (retval.second ? "true" : "false") << std::endl;
  std::cerr << "inserting e2:" << e2 << std::endl;
  retval = edgeS.insert(e2);
  std::cerr << "r.1: " << (*retval.first) << std::endl;
  std::cerr << "r.2 should be true for a correct insertion: " << (retval.second ? "true" : "false") << std::endl;
  std::cerr << "set.begin(): " << (*edgeS.begin()) << std::endl;
  std::cerr << "set.end()  : " << *(--edgeS.end()) << std::endl;

  Point p5 = Point(2,0,5,5);
  Point p6 = Point(2,3,6,6);

  Edge e3 = Edge(&p5, &p6);

  std::cerr << "inserting e3:" << e3 << std::endl;
  retval = edgeS.insert(e3);
  std::cerr << "r.1: " << (*retval.first) << std::endl;
  std::cerr << "r.2 should be true for a correct insertion: " << (retval.second ? "true" : "false") << std::endl;
  std::cerr << "edges in set" << std::endl;
  for (std::set<Edge>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;

  std::cerr << std::endl;

  std::cerr << "=== Determinant test ===" << std::endl;

  bool detBool = false;

  if (detBool) {
    double max=0, current=0.0, area=0;
    unsigned int counter = 0;

    p1.set(current, current);
    p2.set(1.0,1.0);
    e1.set(p1,p2);
    std::cerr.precision(17);

    while (fabs(current - 1.0) > EPSILON) {
      current = nextafter(current, 1.0);
      p1.set(current, current);
      area = det(e1, p1);
      if (area > max) max = area;
      ++counter;
      if (counter % 1000000 == 0) std::cerr << "max: " << max << std::endl;
    }
    std::cerr << "max: " << max << std::endl;
  } else std::cerr << "Det test disabled." << std::endl;

  std::cerr << "=== Verification test ===" << std::endl;

  std::vector<Point> points;
  p1.set(10,10);
  points.push_back(p1);
  p2.set(1,2);
  points.push_back(p2);
  p3.set(2,1);
  points.push_back(p3);
  p4.set(2,2);
  points.push_back(p4);

  //std::vector<unsigned int> polygon;
  //for(int i = 0; i < 4; ++i)
  //  polygon.push_back(i);

  std::vector<unsigned int> ch;

  get_convex_hull(ch, points);
  std::cerr << "ch: " << std::endl;
  pdisplay(ch, points);

/*
  bool btest = verify_simple_polygon(polygon, points);
  std::cerr << "is simple: should be true : " << ((btest) ? "true" : "false") << std::endl;

  polygon[1] = 2;
  polygon[2] = 1;
  btest = verify_simple_polygon(polygon, points);
  std::cerr << "is simple: should be false: " << ((btest) ? "true" : "false") << std::endl;
*/

  std::cerr << std::endl;
  std::cerr << "======= Shewchuks Predicates test ===============" << std::endl;

  point pa, pb, pc, pd;
  pa.x = -0.6034851544931661; pa.y = -0.0572553943219186; //-0.0572554;
  pb.x = -0.6034851421330270; pb.y = -0.0572554178266569; //-0.0572554;
  pc.x = -0.6034829978440949; pc.y = -0.0568532654878436; //-0.0568533;
  pd.x = -0.6034827734680771; pd.y = -0.0568532372913607; //-0.0568532;
  p1.set(-0.6034851544931661, -0.0572553943219186);
  p2.set(-0.6034851421330270, -0.0572554178266569);
  p3.set(-0.6034829978440949, -0.0568532654878436);
  p4.set(-0.6034827734680771, -0.0568532372913607);

  std::cerr << std::setprecision(24);
  std::cerr << "pa.x: " << pa.x << ", pa.y: " << pa.y << std::endl;
  std::cerr << "pb.x: " << pb.x << ", pb.y: " << pb.y << std::endl;
  std::cerr << "pc.x: " << pc.x << ", pc.y: " << pc.y << std::endl;
  std::cerr << "pd.x: " << pd.x << ", pd.y: " << pd.y << std::endl;
  std::cerr << "orient2d(pa,pb,pc): " << orient2d(pa,pb,pc) << std::endl;
  std::cerr << "det(pa, pb, pc)   : " << det(p1, p2, p3) << std::endl;

  pa.x = -0.0003922779898438; pa.y = 0.1603560581152343;
  pb.x = -0.0002011321564974; pb.y = 0.2376256472348480;
  pc.x = -0.0004140803492565; pc.y = 0.1464613835869469;
  pd.x = -0.0002011321556713; pd.y = 0.2376256472341352;

  p1.set(-0.0003922779898438,  0.1603560581152343);
  p2.set(-0.0002011321564974,  0.2376256472348480);
  p3.set(-0.0004140803492565,  0.1464613835869469);
  p4.set(-0.0002011321556713,   0.2376256472341352);
  e1.set(p1,p2);
  e2.set(p3,p4);

  std::cerr << "pa.x: " << pa.x << ", pa.y: " << pa.y << std::endl;
  std::cerr << "pb.x: " << pb.x << ", pb.y: " << pb.y << std::endl;
  std::cerr << "pc.x: " << pc.x << ", pc.y: " << pc.y << std::endl;
  std::cerr << "pd.x: " << pd.x << ", pd.y: " << pd.y << std::endl;

  std::cerr << "det(p1, p2, p3)   : " << det(p1, p2, p3) << std::endl;
  std::cerr << "det(e1, p3)       : " << det(e1, p3) << std::endl;
  std::cerr << "orient2d(pa,pb,pc): " << orient2d(pa,pb,pc) << std::endl;

  std::cerr << "det(p1, p2, p4)   : " << det(p1, p2, p4) << std::endl;
  std::cerr << "det(e1, p4)       : " << det(e1, p4) << std::endl;
  std::cerr << "orient2d(pa,pb,pd): " << orient2d(pa,pb,pd) << std::endl;

  std::cerr << "det(p3, p4, p1)   : " << det(p3, p4, p1) << std::endl;
  std::cerr << "det(e2, p1)       : " << det(e2, p1) << std::endl;
  std::cerr << "orient2d(pc,pd,pa): " << orient2d(pc,pd,pa) << std::endl;

  std::cerr << "det(p3, p4, p2)   : " << det(p3, p4, p2) << std::endl;
  std::cerr << "det(e2, p2)       : " << det(e2, p2) << std::endl;
  std::cerr << "orient2d(pc,pd,pb): " << orient2d(pc,pd,pb) << std::endl;

  // test the time difference between det and orient2d
  //for (unsigned int i = 0; i < 100000000 ; ++i) orient2d(pa,pb,pc);
/*
  std::cerr << "=========== Heap's alg. test ================" << std::endl;

  std::vector<unsigned int> a = {1, 2, 3, 4, 5};
  heap(a, a.size());
*/

}
