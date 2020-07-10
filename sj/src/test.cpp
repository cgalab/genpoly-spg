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
#include <bitset>


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

  std::cerr << std::setprecision(17);
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

  p1.set(-0.3623046875,0.21240234371874361);
  p2.set(-0.36279296875,0.21240234371874361);
  std::cerr << p1 << "  < " << p1 << ", should be false: " << ((p1 < p1) ? "true" : "false") << std::endl;
  std::cerr << p1 << "  > " << p1 << ", should be false: " << ((p1 > p1) ? "true" : "false") << std::endl;
  std::cerr << p1 << " == " << p1 << ", should be true : " << ((p1 == p1) ? "true" : "false") << std::endl;
  std::cerr << p1 << " != " << p1 << ", should be false: " << ((p1 != p1) ? "true" : "false") << std::endl;
  std::cerr << p1 << "  < " << p2 << ", should be false: " << ((p1 < p2) ? "true" : "false") << std::endl;
  std::cerr << p1 << "  > " << p2 << ", should be true : " << ((p1 > p2) ? "true" : "false") << std::endl;
  std::cerr << p1 << " == " << p2 << ", should be false: " << ((p1 == p2) ? "true" : "false") << std::endl;
  std::cerr << p1 << " != " << p2 << ", should be true : " << ((p1 != p2) ? "true" : "false") << std::endl;

  p1.set(-0.5864844941069559,  -0.5519586413083573);
  p2.set(-0.5864844941069558,  -0.6199163586916429);
  std::cerr << p1 << "  < " << p2 << ", should be true: " << ((p1 < p2) ? "true" : "false") << std::endl;
  std::cerr << p1 << "  > " << p2 << ", should be false: " << ((p1 > p2) ? "true" : "false") << std::endl;
  std::cerr << p1 << " == " << p2 << ", should be false : " << ((p1 == p2) ? "true" : "false") << std::endl;
  std::cerr << p1 << " != " << p2 << ", should be true: " << ((p1 != p2) ? "true" : "false") << std::endl;
  std::cerr << p2 << "  < " << p1 << ", should be false: " << ((p2 < p1) ? "true" : "false") << std::endl;
  std::cerr << p2 << "  > " << p1 << ", should be true : " << ((p2 > p1) ? "true" : "false") << std::endl;
  std::cerr << p2 << " == " << p1 << ", should be false: " << ((p2 == p1) ? "true" : "false") << std::endl;
  std::cerr << p2 << " != " << p1 << ", should be true : " << ((p2 != p1) ? "true" : "false") << std::endl;

  std::cerr << std::endl;
  std::cerr << "=== Double Comparisons ===" << std::endl;

  std::cerr << p1.x << " < " << p2.x << ", should be true: " << ((p1.x < p2.x) ? "true" : "false") << std::endl;

  std::cerr << std::endl;
  std::cerr << "=== Edge comparisons ===" << std::endl;

  p1.set(118532,919572,32577,48380,32577);
  p2.set(118532,978866,32578,48379,32578);
  p3.set(2,2,2,2,2);
  p4.set(3,3,3,3,3);
  Edge2 e1 = Edge2(&p1, &p2);
  Edge2 e2 = Edge2(&p3, &p4);
  std::cerr << "(*e1.p1): " << *e1.p1 << " == *p1: " << p1 << " : " <<  ((*e1.p1 == p1) ? "true" : "false") << std::endl;
  std::cerr << "(*e1.p2): " << *e1.p1 << " == *p2: " << p2 << " : " <<  ((*e1.p2 == p2) ? "true" : "false") << std::endl;
  std::cerr << std::endl;

  p1.set(6,4,0,0,0);
  p2.set(6,6,1,1,1);
  p3.set(6,4,0,0,0);
  p4.set(6,7,2,2,2);
  e1.set(p1, p2);
  e2.set(p3, p4);
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;
  std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;
  std::cerr << "e2 < e1: " << ((e2 < e1) ? "true" : "false") << std::endl;

  p1.set(0.1776512115938656,0.59801348903157514,3008,2320,3010);
  p2.set(0.32086761784458201,0.58090142848487081,3415,2321,3415);
  p3.set(0.17057308659368181,0.59801348903163565,2985,2317,2985);
  p4.set(0.1776512115938656,0.59801348903157514,3008,2320,3010);
  e1.set(p1, p2);
  e2.set(p3, p4);
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;
  std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;
  std::cerr << "e2 < e1: " << ((e2 < e1) ? "true" : "false") << std::endl;

  p1.set(-0.1529808807867781, 0.2521944381961845);
  p2.set(-0.1529808806949973, 0.2521944378948504);
  p3.set(-0.1529808807408841, 0.2521944380455210);
  p4.set(-0.1529609603963920, 0.2504762372249587);
  e1.set(p1, p2);
  e2.set(p3, p4);
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;
  std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;
  std::cerr << "e2 < e1: " << ((e2 < e1) ? "true" : "false") << std::endl;

  p1.set(-0.3081799240194343, 0.2746582135433187);
  p2.set(-0.3056010360799846, 0.2746582135433187);
  p3.set(-0.3063961715780295, 0.2746582135433187);
  p4.set(-0.3056010360799846, 0.2746582135433187);
  e1.set(p1, p2);
  e2.set(p3, p4);
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;
  std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;
  std::cerr << "e2 < e1: " << ((e2 < e1) ? "true" : "false") << std::endl;

  p1.set(-0.2943461057385031, -0.3283110598709391);
  p2.set(-0.2939762461102735, -0.3309518576164938);
  p3.set(-0.2943461057385031, -0.3283110598709391);
  p4.set(-0.2940429691043516, -0.3304754554387765);
  e1.set(p1, p2);
  e2.set(p3, p4);
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;
  std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;
  std::cerr << "e2 < e1: " << ((e2 < e1) ? "true" : "false") << std::endl;

  p1.set(-0.1879568629943250, -0.3695956370056750);
  p2.set(-0.1878378004943250, -0.3694765745056750);
  p3.set(-0.1878774879943250, -0.3695162620056750);
  p4.set( 0.3332349999999999,  0.0174609375000000);
  e1.set(p1, p2);
  e2.set(p3, p4);
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;
  std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;
  std::cerr << "e2 < e1: " << ((e2 < e1) ? "true" : "false") << std::endl;

  p1.set(-0.0105778125000000, -0.4809942187500000);
  p2.set(-0.0088315624999999, -0.4809942187500000);
  p3.set(-0.0089903125000000, -0.4809942187500000);
  p4.set( 0.2053618750000000, -0.4999053125000000);
  e1.set(p1, p2);
  e2.set(p3, p4);
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;
  std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;
  std::cerr << "e2 < e1: " << ((e2 < e1) ? "true" : "false") << std::endl;

  p1.set(-0.1682165625000000, -0.2438812500000000);
  p2.set(-0.1678593750000000, -0.2438812500000000);
  p3.set(-0.1680975000000000, -0.2438812500000000);
  p4.set(-0.0615762500000000, -0.2774965625000000);
  e1.set(p1, p2);
  e2.set(p3, p4);
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;
  std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;
  std::cerr << "e2 < e1: " << ((e2 < e1) ? "true" : "false") << std::endl;

  p1.set(-0.1580939378867000, -0.2439324980665031);
  p2.set(-0.1567888746133000, -0.2438928105665031);
  p3.set(-0.1582923753867000, -0.2439721855665031);
  p4.set(-0.1578955003867000, -0.2438928105665031);
  e1.set(p1, p2);
  e2.set(p3, p4);
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;
  std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;
  std::cerr << "e2 < e1: " << ((e2 < e1) ? "true" : "false") << std::endl;

  p1.set(-0.3183227489004278,  0.4305545470863286);
  p2.set(-0.3180112253344731,  0.4293374317588774);
  p3.set(-0.3183227489004278,  0.4305545470863286);
  p4.set(-0.2843666802113348,  0.2978889763940047);
  e1.set(p1, p2);
  e2.set(p3, p4);
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;
  std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;
  std::cerr << "e2 < e1: " << ((e2 < e1) ? "true" : "false") << std::endl;

  p1.set(0.1624848962148917, -0.4609824302726415, 89,0,0);
  p2.set(0.1630934538786164, -0.4615909879363671, 91,0,0);
  p3.set(0.1618763385511670, -0.4603738726089159, 85,0,0);
  p4.set(0.1630934538786164, -0.4615909879363671, 91,0,0);
  e1.set(p1, p2);
  e2.set(p3, p4);
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;
  std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;
  std::cerr << "e2 < e1: " << ((e2 < e1) ? "true" : "false") << std::endl;

  p1.set(0.1618763385511670, -0.4603738726089159, 85,0,0);
  p2.set(0.1630934538786164, -0.4615909879363671, 91,0,0);
  p3.set(0.1624848962148917, -0.4609824302726415, 89,0,0);
  p4.set(0.1637020115423411, -0.4981044477599457, 97,0,0);
  e1.set(p1, p2);
  e2.set(p3, p4);
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;
  std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;
  std::cerr << "e2 < e1: " << ((e2 < e1) ? "true" : "false") << std::endl;

  p1.set(-0.4084123883928586, -0.1114676339285719,885,0,0);
  p2.set(-0.4056222098214299, -0.0365513392857152,906,0,0);
  p3.set(-0.4101562500000014, -0.1149553571428578,876,0,0);
  p4.set(-0.4066685267857159, -0.1079799107142863,896,0,0);
  e1.set(p1, p2);
  e2.set(p3, p4);
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;
  std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;
  std::cerr << "e2 < e1: " << ((e2 < e1) ? "true" : "false") << std::endl;

  p1.set(-0.4084123883928586, -0.1114676339285719,885,0,0);
  p2.set(-0.4066685267857159, -0.1079799107142863,896,0,0);
  p3.set(-0.4101562500000014, -0.1149553571428578,876,0,0);
  p4.set(-0.4066685267857159, -0.1079799107142863,896,0,0);
  e1.set(p1, p2);
  e2.set(p3, p4);
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;
  std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;
  std::cerr << "e2 < e1: " << ((e2 < e1) ? "true" : "false") << std::endl;

  p1.set(0.2600000000000000,  0.4040000000000000,1167,0,0);
  p2.set(0.2760000000000000,  0.3960000000000000,1210,0,0);
  p3.set(0.2680000000000000,  0.4000000000000000,1186,0,0);
  p4.set(0.3400000000000000,  0.3640000000000000,1427,0,0);
  e1.set(p1, p2);
  e2.set(p3, p4);
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;
  std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;
  std::cerr << "e2 < e1: " << ((e2 < e1) ? "true" : "false") << std::endl;

  p1.set(-0.048828125000000,  0.4865722656250000,23769,0,0);
  p2.set(-0.046386718750000,  0.4807128906250000,23865,0,0);
  p3.set(-0.048828125000000,  0.4865722656250000,23769,0,0);
  p4.set(0.3793945312500000,  0.1247558593750000,40967,0,0);
  e1.set(p1, p2);
  e2.set(p3, p4);
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;
  std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;
  std::cerr << "e2 < e1: " << ((e2 < e1) ? "true" : "false") << std::endl;

  p1.set(-0.048828125000000,  0.4865722656250000,23769,0,0);
  p2.set(-0.046386718750000,  0.4807128906250000,23865,0,0);
  p3.set(-0.046875000000000,  0.4846191406250000,23845,0,0);
  p4.set(-0.044921875000000,  0.4797363281250000,23919,0,0);
  e1.set(p1, p2);
  e2.set(p3, p4);
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;
  std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;
  std::cerr << "e2 < e1: " << ((e2 < e1) ? "true" : "false") << std::endl;

  p1.set(-0.048828125000000,  0.4865722656250000,23769,0,0);
  p2.set(0.3793945312500000,  0.1247558593750000,40967,0,0);
  p3.set(-0.046875000000000,  0.4846191406250000,23845,0,0);
  p4.set(-0.044921875000000,  0.4797363281250000,23919,0,0);
  e1.set(p1, p2);
  e2.set(p3, p4);
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;
  std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;
  std::cerr << "e2 < e1: " << ((e2 < e1) ? "true" : "false") << std::endl;

  p1.set(-0.048828125000000,  0.4865722656250000,23769,0,0);
  p2.set(-0.046386718750000,  0.4807128906250000,23865,0,0);
  p3.set(-0.046875000000000,  0.4846191406250000,23845,0,0);
  p4.set(-0.046386718750000,  0.4841308593750000,23866,0,0);
  e1.set(p1, p2);
  e2.set(p3, p4);
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;
  std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;
  std::cerr << "e2 < e1: " << ((e2 < e1) ? "true" : "false") << std::endl;

  p1.set(-0.048828125000000,  0.4865722656250000,23769,0,0);
  p2.set(0.3793945312500000,  0.1247558593750000,40967,0,0);
  p3.set(-0.046875000000000,  0.4846191406250000,23845,0,0);
  p4.set(-0.046386718750000,  0.4841308593750000,23866,0,0);
  e1.set(p1, p2);
  e2.set(p3, p4);
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;
  std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;
  std::cerr << "e2 < e1: " << ((e2 < e1) ? "true" : "false") << std::endl;

  p1.set(-0.55224609375,0.006103515625, 26,0,0);
  p2.set(-0.1552734375,0.259033203125, 18195,0,0);
  p3.set(-0.552734375,0.006103515625, 21,0,0);
  p4.set(-0.001953125,0.371337890625, 25348,0,0);
  e1.set(p1, p2);
  e2.set(p3, p4);
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;
  std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;
  std::cerr << "e2 < e1: " << ((e2 < e1) ? "true" : "false") << std::endl;

  p1.set(-0.55224609375,0.006103515625, 26,0,0);
  p2.set(-0.1552734375,0.259033203125, 18195,0,0);
  p3.set(-0.552734375,0.006103515625, 21,0,0);
  p4.set(0.22705078125,-0.086181640625, 36131,0,0);
  e1.set(p1, p2);
  e2.set(p3, p4);
  std::cerr << "e1: " << e1 << std::endl;
  std::cerr << "e2: " << e2 << std::endl;
  std::cerr << "e1 < e2: " << ((e1 < e2) ? "true" : "false") << std::endl;
  std::cerr << "e2 < e1: " << ((e2 < e1) ? "true" : "false") << std::endl;

  std::cerr << std::endl;
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
  std::cout << "intersection: " << e1 << " with " << e2 << ", should be true: " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";
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
  std::cout << "intersection: " << e1 << " with " << e2 << ", should be true: " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";
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

  p1.set(0.1919999999999999, -0.3560000000000000, 1041, 0, 0);
  p2.set(0.2040000000000000,  -0.3480000000000000, 1052, 0, 0);
  e1.set(p1,p2);
  p3.set(0.1919999999999999,  -0.3600000000000000, 1040, 0, 0);
  p4.set(0.2120000000000000,  -0.3400000000000000, 1063, 0, 0);
  e2.set(p3,p4);

  itest = checkIntersection(e1, e2);;
  std::cout << "intersection: " << e1 << " with " << e2 << ", should be true: " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";printEnum(itest);std::cerr << std::endl;
  itest = checkIntersection(e2, e1);
  std::cout << "intersection: " << e2 << " with " << e1 << ", should be true: " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";printEnum(itest);std::cerr << std::endl;

  p1.set(-0.0640000000000000, 0.124000000000000, 490, 0, 0);
  p2.set( 0.0320000000000000, 0.028000000000000, 717, 0, 0);
  e1.set(p1,p2);
  p3.set(-0.0680000000000000, 0.128000000000000, 482,0,0);
  p4.set( 0.0120000000000000, 0.048000000000000, 665,0,0);
  e2.set(p3,p4);
  std::cerr << std::endl;
  itest = checkIntersection(e1, e2);
//  std::cout << "*************************" << std::endl;
//  std::cout << "e1.cdet(*e2.p1): " << e1.cdet(*e2.p1) << std::endl;
//  std::cout << "e1.cdet(*e2.p2): " << e1.cdet(*e2.p2) << std::endl;
//  std::cout << "e2.cdet(*e1.p1): " << e2.cdet(*e1.p1) << std::endl;
//  std::cout << "e2.cdet(*e1.p2): " << e2.cdet(*e1.p2) << std::endl;
  std::cout << "intersection: " << e1 << " with " << e2 << ", should be true: " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";printEnum(itest);std::cerr << std::endl;
  itest = checkIntersection(e2, e1);
  std::cout << "intersection: " << e2 << " with " << e1 << ", should be true: " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";printEnum(itest);std::cerr << std::endl;

  p1.set(0.5566406250000000, 0.1640625000000000, 2504, 0, 0);
  p2.set(0.5605468750000000, 0.1562500000000000, 2508, 0, 0);
  e1.set(p1,p2);
  p3.set(0.5488281250000000, 0.1523437500000000, 2496,0,0);
  p4.set(0.5605468750000000, 0.1679687500000000, 2510,0,0);
  e2.set(p3,p4);
  std::cerr << std::endl;
  itest = checkIntersection(e1, e2);
  std::cout << "intersection: " << e1 << " with " << e2 << ", should be true: " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";printEnum(itest);std::cerr << std::endl;

  p1.set(0.5488281250000000, 0.1523437500000000, 2496, 0, 0);
  p2.set(0.5605468750000000, 0.1562500000000000, 2508, 0, 0);
  e1.set(p1,p2);
  p3.set(0.4902343750000000, 0.0273437500000000, 2452,0,0);
  p4.set(0.5605468750000000, 0.1679687500000000, 2510,0,0);
  e2.set(p3,p4);
  std::cerr << std::endl;
  itest = checkIntersection(e1, e2);
  std::cout << "intersection: " << e1 << " with " << e2 << ", should be true: " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";printEnum(itest);std::cerr << std::endl;

  p1.set(7.000000, 1.000000, 36, 0, 0);
  p2.set(3.000000, 1.000000, 37, 0, 0);
  e1.set(p1,p2);
  p3.set(5.000000, 1.000000, 0,0,0);
  p4.set(6.000000, 1.000000, 1,0,0);
  e2.set(p3,p4);
  std::cerr << std::endl;
  itest = checkIntersection(e1, e2);
  std::cout << "intersection: " << e1 << " with " << e2 << ", should be true: " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";printEnum(itest);std::cerr << std::endl;

  p1.set(0.476,-0.026, 1066, 0, 0);
  p2.set(0.478,-0.016, 1070, 0, 0);
  e1.set(p1,p2);
  p3.set(0.472,-0.046, 1058,0,0);
  p4.set(0.482,0.004, 1078,0,0);
  e2.set(p3,p4);
  std::cerr << std::endl;
  itest = checkIntersection(e1, e2);
  std::cout << "intersection: " << e1 << " with " << e2 << ", should be false: " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";printEnum(itest);std::cerr << std::endl;
  std::cout << "e1.cdet(p3): " << e1.cdet(p3) << ", e1.cdet(p4): " << e1.cdet(p4) << std::endl;
  std::cout << "e2.cdet(p1): " << e2.cdet(p1) << ", e2.cdet(p2): " << e2.cdet(p2) << std::endl;
  std::cout << "multiplied together: " << e1.cdet(p3)*e1.cdet(p4)*e2.cdet(p1)*e2.cdet(p2) << ", ==epsilon?: " << ((e1.cdet(p3)*e1.cdet(p4)*e2.cdet(p1)*e2.cdet(p2) < EPSILON) ? "true" : "false")<< std::endl;
  std::cout << "e1.det(p3): " << e1.det(p3) << ", e1.det(p4): " << e1.det(p4) << std::endl;
  std::cout << "e2.det(p1): " << e2.det(p1) << ", e2.det(p2): " << e2.det(p2) << std::endl;
  std::cout << "multiplied together: " << e1.det(p3)*e1.det(p4)*e2.det(p1)*e2.det(p2) << ", ==epsilon?: " << ((e1.cdet(p3)*e1.cdet(p4)*e2.cdet(p1)*e2.cdet(p2) < EPSILON) ? "true" : "false")<< std::endl;

  p1.set(0.476,-0.026, 1066, 0, 0);
  p2.set(0.482,0.004, 1078, 0, 0);
  e1.set(p1,p2);
  p3.set(0.472,-0.046, 1058,0,0);
  p4.set(0.478,-0.016, 1070,0,0);
  e2.set(p3,p4);
  std::cerr << std::endl;
  itest = checkIntersection(e1, e2);
  std::cout << "intersection: " << e1 << " with " << e2 << ", should be true: " << ((itest >= IS_TRUE) ? "true" : "false") << " , value: ";printEnum(itest);std::cerr << std::endl;
  std::cout << "e1.cdet(p3): " << e1.cdet(p3) << ", e1.cdet(p4): " << e1.cdet(p4) << std::endl;
  std::cout << "e2.cdet(p1): " << e2.cdet(p1) << ", e2.cdet(p2): " << e2.cdet(p2) << std::endl;

/*
  Testing Yval class in edge.h
*/
  std::cerr << std:: endl;
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
  //y1 = getYatX(e1, 49720);
  //y2 = getYatX(e2, 49720);
  //std::cerr << std::setprecision(15) << y1 << " == " << y2 << " should be true : " << ((y1 == y2) ? "true" : "false") << std::endl;

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

  std::cerr << e1 << " == " << e2 << " should be true : " << ((e1 == e2) ? "true" : "false") << std::endl;

  std::cerr << std::endl;
  std::cerr << "=== Edge set tests ===" << std::endl;

  std::set<Edge2> edgeS; // a set of edges
  std::pair<std::set<Edge2>::iterator,bool> retval; // return value

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

  Edge2 e3 = Edge2(&p5, &p6);

  std::cerr << "inserting e3:" << e3 << std::endl;
  retval = edgeS.insert(e3);
  std::cerr << "r.1: " << (*retval.first) << std::endl;
  std::cerr << "r.2 should be true for a correct insertion: " << (retval.second ? "true" : "false") << std::endl;
  std::cerr << "edges in set" << std::endl;
  for (std::set<Edge2>::iterator it=edgeS.begin(); it!=edgeS.end(); ++it) std::cerr << *it << std::endl;

  std::cerr << std::endl;

  std::cerr << "=== Determinant test ===" << std::endl;

  p1.set(-0.4410479999999999956,0.30967199999999989179);
  p2.set(-0.43791999999999997595,0.30967199999999989179);
  e1.set(p1,p2);
  p3.set(-0.45356000000000001871,0.31280000000000002247);
  double area = e1.cdet(p3);
  std::cerr << "e1: " << e1 << ", p3: " << p3 << std::endl;
  std::cerr << "det: " << area  << ", det != 0: " << ((area != 0) ? "true" : "false") << std::endl;



  bool detBool = false;

  if (detBool) {
    double max=0, current=0.0;
    unsigned int counter = 0;

    p1.set(current, current);
    p2.set(1.0,1.0);
    e1.set(p1,p2);
    std::cerr.precision(17);

    while (fabs(current - 1.0) > EPSILON) {
      current = nextafter(current, 1.0);
      p1.set(current, current);
      area = e1.cdet(p1);
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
  std::cerr << "======= Shewchuks Predicates test 1 ===============" << std::endl;

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
  std::cerr << "det(e1, p3)       : " << e1.cdet(p3) << std::endl;
  std::cerr << "orient2d(pa,pb,pc): " << orient2d(pa,pb,pc) << std::endl;

  std::cerr << "det(p1, p2, p4)   : " << det(p1, p2, p4) << std::endl;
  std::cerr << "det(e1, p4)       : " << e1.cdet(p4) << std::endl;
  std::cerr << "orient2d(pa,pb,pd): " << orient2d(pa,pb,pd) << std::endl;

  std::cerr << "det(p3, p4, p1)   : " << det(p3, p4, p1) << std::endl;
  std::cerr << "det(e2, p1)       : " << e2.cdet(p1) << std::endl;
  std::cerr << "orient2d(pc,pd,pa): " << orient2d(pc,pd,pa) << std::endl;

  std::cerr << "det(p3, p4, p2)   : " << det(p3, p4, p2) << std::endl;
  std::cerr << "det(e2, p2)       : " << e2.cdet(p2) << std::endl;
  std::cerr << "orient2d(pc,pd,pb): " << orient2d(pc,pd,pb) << std::endl;

  std::cerr << "======= Shewchuks Predicates test 2 ===============" << std::endl;

  pa.x = 1; pa.y = 1;
  pb.x = 2; pb.y = 2;
  pc.x = 2; pc.y = 1;
  pd.x = 1; pd.y = 2;
  p1.set(1, 1);
  p2.set(2, 2);
  p3.set(2, 1);
  p4.set(1, 2);

  e1.set(p1,p2);
  e2.set(p3,p4);

  std::cerr << "pa.x: " << pa.x << ", pa.y: " << pa.y << std::endl;
  std::cerr << "pb.x: " << pb.x << ", pb.y: " << pb.y << std::endl;
  std::cerr << "pc.x: " << pc.x << ", pc.y: " << pc.y << std::endl;
  std::cerr << "pd.x: " << pd.x << ", pd.y: " << pd.y << std::endl;

  std::cerr << "det(p1, p2, p3)   : " << det(p1, p2, p3) << std::endl;
  std::cerr << "det(e1, p3)       : " << e1.cdet(p3) << std::endl;
  std::cerr << "orient2d(pa,pb,pc): " << orient2d(pa,pb,pc) << std::endl;

  std::cerr << "det(p1, p2, p4)   : " << det(p1, p2, p4) << std::endl;
  std::cerr << "det(e1, p4)       : " << e1.cdet(p4) << std::endl;
  std::cerr << "orient2d(pa,pb,pd): " << orient2d(pa,pb,pd) << std::endl;

  std::cerr << "det(p3, p4, p1)   : " << det(p3, p4, p1) << std::endl;
  std::cerr << "det(e2, p1)       : " << e2.cdet(p1) << std::endl;
  std::cerr << "orient2d(pd,pc,pa): " << orient2d(pd,pc,pa) << std::endl;

  std::cerr << "det(p3, p4, p2)   : " << det(p3, p4, p2) << std::endl;
  std::cerr << "det(e2, p2)       : " << e2.cdet(p2) << std::endl;
  std::cerr << "orient2d(pd,pc,pb): " << orient2d(pd,pc,pb) << std::endl;

  std::cerr << "reldist(e1, p3): " << reldist(e1, p3) << std::endl;
  std::cerr << "reldist(e1, p4): " << reldist(e1, p4) << std::endl;
  std::cerr << "reldist(e2, p1): " << reldist(e2, p1) << std::endl;
  std::cerr << "reldist(e2, p2): " << reldist(e2, p2) << std::endl;


  pa.x = 1; pa.y = 1;
  pb.x = 2; pb.y = 2;
  pc.x = 1; pc.y = 1;
  pd.x = 1; pd.y = 2;
  p1.set(1, 1);
  p2.set(2, 2);
  p3.set(1, 1);
  p4.set(1, 2);

  e1.set(p1,p2);
  e2.set(p3,p4);

  std::cerr << "pa.x: " << pa.x << ", pa.y: " << pa.y << std::endl;
  std::cerr << "pb.x: " << pb.x << ", pb.y: " << pb.y << std::endl;
  std::cerr << "pc.x: " << pc.x << ", pc.y: " << pc.y << std::endl;
  std::cerr << "pd.x: " << pd.x << ", pd.y: " << pd.y << std::endl;

  std::cerr << "det(p1, p2, p3)   : " << det(p1, p2, p3) << std::endl;
  std::cerr << "det(e1, p3)       : " << e1.cdet(p3) << std::endl;
  std::cerr << "orient2d(pa,pb,pc): " << orient2d(pa,pb,pc) << std::endl;
  std::cerr << "orient2d(pa,pb,pd): " << orient2d(pa,pb,pd) << std::endl;
  std::cerr << "orient2d(pc,pd,pa): " << orient2d(pc,pd,pa) << std::endl;
  std::cerr << "orient2d(pc,pd,pb): " << orient2d(pc,pd,pb) << std::endl;

  pa.x = 1; pa.y = 1;
  pb.x = 2; pb.y = 2;
  pc.x = 1; pc.y = 2;
  pd.x = 2; pd.y = 2;
  p1.set(1, 1);
  p2.set(2, 2);
  p3.set(1, 2);
  p4.set(2, 2);

  e1.set(p1,p2);
  e2.set(p3,p4);

  std::cerr << "pa.x: " << pa.x << ", pa.y: " << pa.y << std::endl;
  std::cerr << "pb.x: " << pb.x << ", pb.y: " << pb.y << std::endl;
  std::cerr << "pc.x: " << pc.x << ", pc.y: " << pc.y << std::endl;
  std::cerr << "pd.x: " << pd.x << ", pd.y: " << pd.y << std::endl;

  std::cerr << "det(p1, p2, p3)   : " << det(p1, p2, p3) << std::endl;
  std::cerr << "det(e1, p3)       : " << e1.cdet(p3) << std::endl;
  std::cerr << "orient2d(pa,pb,pc): " << orient2d(pa,pb,pc) << std::endl;
  std::cerr << "orient2d(pa,pb,pd): " << orient2d(pa,pb,pd) << std::endl;
  std::cerr << "orient2d(pc,pd,pa): " << orient2d(pc,pd,pa) << std::endl;
  std::cerr << "orient2d(pc,pd,pb): " << orient2d(pc,pd,pb) << std::endl;

  //========================

  pa.x = -0.36279296875; pa.y = 0.21240234371874361;
  pb.x = -0.3623046875;  pb.y = 0.21240234371874361;
  pc.x = -0.36328125;    pc.y = 0.21240234371874361;
  pd.x = -0.361328125;   pd.y = 0.21240234371874361;
  p1.set(-0.36279296875, 0.21240234371874361);
  p2.set(-0.3623046875,  0.21240234371874361);
  p3.set(-0.36328125,    0.21240234371874361);
  p4.set(-0.361328125,   0.21240234371874361);

  e1.set(p1,p2);
  e2.set(p3,p4);

  std::cerr << "pa.x: " << pa.x << ", pa.y: " << pa.y << std::endl;
  std::cerr << "pb.x: " << pb.x << ", pb.y: " << pb.y << std::endl;
  std::cerr << "pc.x: " << pc.x << ", pc.y: " << pc.y << std::endl;
  std::cerr << "pd.x: " << pd.x << ", pd.y: " << pd.y << std::endl;

  std::cerr << "det(p1, p2, p3)   : " << det(p1, p2, p3) << std::endl;
  std::cerr << "det(e1, p3)       : " << e1.cdet(p3) << std::endl;
  std::cerr << "orient2d(pa,pb,pc): " << orient2d(pa,pb,pc) << std::endl;

  std::cerr << "det(p1, p2, p4)   : " << det(p1, p2, p4) << std::endl;
  std::cerr << "det(e1, p4)       : " << e1.cdet(p4) << std::endl;
  std::cerr << "orient2d(pa,pb,pd): " << orient2d(pa,pb,pd) << std::endl;

  std::cerr << "det(p3, p4, p1)   : " << det(p3, p4, p1) << std::endl;
  std::cerr << "det(e2, p1)       : " << e2.cdet(p1) << std::endl;
  std::cerr << "orient2d(pd,pc,pa): " << orient2d(pd,pc,pa) << std::endl;

  std::cerr << "det(p3, p4, p2)   : " << det(p3, p4, p2) << std::endl;
  std::cerr << "det(e2, p2)       : " << e2.cdet(p2) << std::endl;
  std::cerr << "orient2d(pd,pc,pb): " << orient2d(pd,pc,pb) << std::endl;

  std::cerr << "reldist(e1, p3): " << reldist(e1, p3) << std::endl;
  std::cerr << "reldist(e1, p4): " << reldist(e1, p4) << std::endl;
  std::cerr << "reldist(e2, p1): " << reldist(e2, p1) << std::endl;
  std::cerr << "reldist(e2, p2): " << reldist(e2, p2) << std::endl;

  pa.x = 0;   pa.y = 0.4;
  pb.x = 0.8; pb.y = 0.9;
  pc.x = 0;   pc.y = 0.6;
  pd.x = 0;   pd.y = 1;
  p1.set(0,   0.4);
  p2.set(0.8, 0.9);
  p3.set(0,   0.6);
  p4.set(0,   1);

  e1.set(p1,p2);
  e2.set(p3,p4);

  std::cerr << "pa.x: " << pa.x << ", pa.y: " << pa.y << std::endl;
  std::cerr << "pb.x: " << pb.x << ", pb.y: " << pb.y << std::endl;
  std::cerr << "pc.x: " << pc.x << ", pc.y: " << pc.y << std::endl;
  std::cerr << "pd.x: " << pd.x << ", pd.y: " << pd.y << std::endl;

  std::cerr << "det(p1, p2, p3)   : " << det(p1, p2, p3) << std::endl;
  std::cerr << "det(e1, p3)       : " << e1.cdet(p3) << std::endl;
  std::cerr << "orient2d(pa,pb,pc): " << orient2d(pa,pb,pc) << std::endl;

  std::cerr << "det(p1, p2, p4)   : " << det(p1, p2, p4) << std::endl;
  std::cerr << "det(e1, p4)       : " << e1.cdet(p4) << std::endl;
  std::cerr << "orient2d(pa,pb,pd): " << orient2d(pa,pb,pd) << std::endl;

  std::cerr << "det(p3, p4, p1)   : " << det(p3, p4, p1) << std::endl;
  std::cerr << "det(e2, p1)       : " << e2.cdet(p1) << std::endl;
  std::cerr << "orient2d(pc,pd,pa): " << orient2d(pc,pd,pa) << std::endl;

  std::cerr << "det(p3, p4, p2)   : " << det(p3, p4, p2) << std::endl;
  std::cerr << "det(e2, p2)       : " << e2.cdet(p2) << std::endl;
  std::cerr << "orient2d(pc,pd,pb): " << orient2d(pc,pd,pb) << std::endl;

  std::cerr << "reldist(e1, p3): " << reldist(e1, p3) << std::endl;
  std::cerr << "reldist(e1, p4): " << reldist(e1, p4) << std::endl;
  std::cerr << "reldist(e2, p1): " << reldist(e2, p1) << std::endl;
  std::cerr << "reldist(e2, p2): " << reldist(e2, p2) << std::endl;

  std::cerr << std::endl;
  std::cerr << "=== angle tests ===" << std::endl;

  p1.set(3, 4);
  p2.set(1, 5);
  p3.set(1, 4);

  double ang1 = atan2(p2.y - p1.y, p2.x - p1.x);
  double ang2 = atan2(p3.y - p1.y, p3.x - p1.x);
  double delta = ang2 - ang1;
  std::cerr << "ang1: " << ang1 << ", ang2: " << ang2 << ", delta: " << delta << std::endl;

  std::cerr << "=== misc tests ===" << std::endl;

  std::cerr << "0*-1" << " == " << 0 << " : " << ((0*-1 == 0) ? "true" : "false") << std::endl;
  std::cerr << "-0.0" << " == " << 0 << " : " << ((-0.0 == 0) ? "true" : "false") << std::endl;


  // test the time difference between det and orient2d
  //for (unsigned int i = 0; i < 100000000 ; ++i) orient2d(pa,pb,pc);
/*
  std::cerr << std::endl;
  std::cerr << "============== equality bit check =================" << std::endl;

  double x,y;
  x = -0.6321507964350201;
  y = -0.6321507964348736;

  //std::cerr.precision(24);
  std::cerr << "x: " << x << ", y: " << y << std::endl;
  std::cerr << "bitset version: " << std::endl;
  std::cerr << "x: " << std::bitset<sizeof(double) * 8> (*(reinterpret_cast<unsigned long long*>(&x))) << ", y: " << std::bitset<sizeof(double) * 8> (*(reinterpret_cast<unsigned long long*>(&y))) << std::endl;
*/
/*
  std::cerr << "=========== Heap's alg. test ================" << std::endl;

  std::vector<unsigned int> a = {1, 2, 3, 4, 5};
  heap(a, a.size());
*/

}
