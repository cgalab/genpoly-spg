#include <vector>
#include <list>
#include <algorithm>
#include "edge.h"
#include "point.h"
#include "random.h"

void createRandPol(std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  unsigned int i, j, k;
  polygon.resize(points.size());

  for(unsigned int j = 0; j < polygon.size();++j) {
    polygon[j] = points[j].i;
  }

  for (i = polygon.size()-1; i > 0; --i) {
     UniformRandomI(j, 0, i);
     k = polygon[i];
     polygon[i] = polygon[j];
     polygon[j] = k;
     points[polygon[i]].v = i;
  }
  points[polygon[i]].v = i;
}

// Sort the points vector into lexicographical order
void lexSort(std::vector<unsigned int>& lex, std::vector<Point>& points) {
  std::sort(lex.begin(), lex.end(), lexComp(points));
}

// checks if 2 edges intersect, returns true if they intersect.
bool checkIntersection(const Edge e1, const Edge e2) {
  // 2 edges with a common point cannot intersect.
  std::cout << "IE: e1: " << e1 << std::endl;
  std::cout << "IE: e2: " << e2 << std::endl;
  if(((*e1.p1).v == (*e2.p1).v) || ((*e1.p1).v == (*e2.p2).v) || ((*e1.p2).v == (*e2.p1).v) || ((*e1.p2).v == (*e2.p2).v))
    return false;

  const Point& pa = *e1.p1;
  const Point& pb = *e1.p2;
  const Point& pc = *e2.p1;
  const Point& pd = *e2.p2;

  double s = 0, t = 0;
  double s_num =  ( (pc.x - pa.x) * (pb.y - pa.y) + (pa.y - pc.y) * (pb.x - pa.x) );
  double s_den =  ( (pd.y - pc.y) * (pb.x - pa.x) - (pd.x - pc.x) * (pb.y - pa.y) );
  double t_num =  ( s * (pd.x - pc.x) + (pc.x - pa.x) );
  double t_den =  ( (pb.x - pa.x) );
  std::cout << "s_num: " << s_num << ", s_den: " << s_den << ", t_num: " << t_num << ", t_den: " << t_den << std::endl;

  s = s_num / s_den;             
  t = t_num / t_den;

  std::cout << "s: " << s << " t: " << t << std::endl;

  if ( ((s >= (double) 0) && (s <= (double) 1)) && ((t >= (double) 0) && (t <= (double) 1)) )
  {
    // cout << "Intersecting edges, abort.\n";
    return true;
  }
  return false;
}

// flips the smaller range of indexes in a polygon and updates the references in 'points' vector
void flip(Edge& e1, Edge& e2, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  //std::cout << "inside flip" << std::endl;
  
  // edge case, one edge goes from index 0 to last index 
  if ( (((*e1.p1).v ==0) && ((*e1.p2).v == points.size()-1)) || (((*e2.p1).v == 0) && ((*e2.p2).v == points.size()-1)) ) {
    //std::cout << "one edge is on the boundary" << std::endl;
    Edge middle, boundary;
    middle = ((*e1.p1).v == 0) ? e2 : e1;
    boundary = (middle == e1) ? e2 : e1;
    //std::cout << "middle: " << middle << std::endl;
    //std::cout << "boundary: " << boundary << std::endl;

    unsigned int lower, higher;
    lower = (*middle.p1).v +1;
    higher = points.size() - (*middle.p2).v;
    //std::cout << "lower: " << lower << std::endl;
    //std::cout << "higher: " << higher << std::endl;

    if (lower < higher) doFlip(0, (*middle.p1).v, polygon, points);
    else doFlip((*middle.p2).v, points.size()-1, polygon, points);

  }
  else {
    //std::cout << "both edges are inside boundary" << std::endl;
    Edge lower, higher;
    lower = ((*e1.p2).v < (*e2.p1).v) ? e1 : e2;
    higher = (lower == e1) ? e2 : e1;
    //std::cout << "lower: " << lower << std::endl;
    //std::cout << "higher: " << higher << std::endl;

    unsigned int inner, outer;
    inner = (*higher.p1).v - (*lower.p2).v +1;
    outer = (points.size() - (*higher.p2).v) + (*lower.p1).v +1;
    //std::cout << "inner: " << inner << ", outer: " << outer << std::endl;

    if (inner < outer) doFlip((*lower.p2).v, (*higher.p1).v, polygon, points);
    else doFlip((*higher.p2).v, (*lower.p1).v, polygon, points);
  }
}

void doFlip(unsigned int i1, unsigned int i2, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  //std::cout << "inside doFlip" << std::endl;
  //std::cout << "flipping: i1: " << i1 << ", i2: " << i2 << std::endl;

  unsigned int t;
  if (i1 < i2) {
    while (i1 < i2) {
      //std::cout << "i1: " << i1 << ", i2: " << i2 << std::endl;
      //std::cout << "p[i1]: " << polygon[i1] << ", p[i2]: " << polygon[i2] << std::endl;
      //std::cout << "p[p[i1]].v: " << points[polygon[i1]].v << ", p[p[i2]].v: " << points[polygon[i2]].v << std::endl;
      t = polygon[i1];
      polygon[i1] = polygon[i2];
      polygon[i2] = t;
      //std::cout << "p[i1]:" << polygon[i1] << ", p[i2]:" << polygon[i2] << std::endl;
      points[polygon[i1]].v = i1;
      points[polygon[i2]].v = i2;
      //std::cout << "p[p[i1]].v:" << points[polygon[i1]].v << ", p[p[i2]].v:" << points[polygon[i2]].v << std::endl;
      ++i1;
      --i2;
      //std::cout << "new i1:" << i1 << std::endl;
      //std::cout << "new i2:" << i2 << std::endl;
    }
  }
  else { // if i1 is higher than i2 we flip the outer polygonal chain
    i2 += points.size();
    //std::cout << "new i2: " << i2 << std::endl;

    while(i1 < i2) {
      //std::cout << "i1: " << i1 << ", i2: " << i2 << std::endl;;
      //std::cout << "mod of i1: " << (i1 % points.size()) << std::endl;
      //std::cout << "mod of i2: " << (i2 % points.size()) << std::endl;
      //std::cout << "p[i1]: " << polygon[(i1 % points.size())] << ", p[i2]: " << polygon[(i2 % points.size())] << std::endl;
      //std::cout << "p[p[i1]]: " << points[polygon[(i1 % points.size())]].v << ", p[p[i2]]: " << points[polygon[(i2 % points.size())]].v << std::endl;
      t = polygon[(i1 % points.size())];
      polygon[(i1 % points.size())] = polygon[(i2 % points.size())];
      polygon[(i2 % points.size())] = t;
      //std::cout << "p[i1]:" << polygon[(i1 % points.size())] << ", p[i2]:" << polygon[(i2 % points.size())] << std::endl;
      points[polygon[(i1 % points.size())]].v = i1 % points.size();
      points[polygon[(i2 % points.size())]].v = i2 % points.size();
      //std::cout << "p[p[i1]].v:" << points[polygon[(i1 % points.size())]].v << ", p[p[i2]].v:" << points[polygon[(i2 % points.size())]].v << std::endl;
      ++i1;
      --i2;
      //std::cout << "new i1:" << i1 << std::endl;
      //std::cout << "new i2:" << i2 << std::endl;
    }
  }
}

// simple function to print out a polygon
void poldisplay (std::vector<unsigned int>& p) {
  for (unsigned int i = 0; i < p.size();++i) {
    std::cout << p[i] << std::endl;
  }  
  std::cout  << std::endl; 
}