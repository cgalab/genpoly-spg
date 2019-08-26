#include <iostream> // for cout, cerr and endl
#include <vector>
#include <algorithm>    // for std::upper_bound and std::sort
#include <math.h>       /* sqrt */
#include "basicDefinitions.h"
#include "point.h"
#include "rand.h"

/*
  a new version of quickstar
INPUT:
  polygon: a vector of indices (unsigned int) into a vector of 'Point's
  points:   vector of class 'Point'
  randseed:  an unsigned integer needed to determine where the 'center' should be within the convex hull.

*/
enum error star(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed) {
  enum error retval = SUCCESS;

  if (randseed) mt.seed(randseed);

  // To pick a random point inside for the 'center' point:
  // I need the convex hull
  std::vector<unsigned int> ch;
  get_convex_hull(ch, points, true);

  // With the convex hull, I can use the first point, and go through each other 2 points and calculate the area.
  // as I only want to know which triangle the point should fall into, I can have a vector of doubles with the total sum of the triangles so far
  // I can use the det function, no harm in that it is the
  std::vector<double> t_area;
  double area = 0.0;

  Point p0 = points[ch[0]];
  Point p1, p2;
  for (unsigned int i = 2; i < ch.size();++i) {
    p1 = points[ch[i-1]];
    p2 = points[ch[i]];
    area = area + det(p0, p1, p2);
    t_area.push_back(area);
  }

//  for (unsigned int i=0; i < t_area.size();++i) {
//    std::cerr << "area: " << t_area[i] << std::endl;
//  }

  double rand_p;
  UniformRandomD(rand_p, 0, area);
//  std::cerr << "random point: " << rand_p << std::endl;

  std::vector<double>::iterator up;
  up=std::upper_bound (t_area.begin(), t_area.end(), rand_p);
  // position is: (up - t_area.begin())

  unsigned int pos = (int)(up - t_area.begin());
//  std::cerr << "upper bound index: " << pos << ", upper bound point: " << points[ch[pos+2]] << std::endl;

  p2 = points[ch[pos+2]];
  p1 = points[ch[pos+1]];
  //p0 already defined

  // then grab 2 uniformly random numbers between 0 and the total area,
  double r1, r2;
  UniformRandomD(r1, 0, 1.0);
  UniformRandomD(r2, 0, 1.0);

  //then use the formula from "Graphics Gems" P = (1-sqrt(r1)) * A + (sqrt(r1) * (1-sqrt(r2))) * B + sqrt(r1)*r2 * C
  // to find the point itself.
  double x, y;
  x = (1-sqrt(r1)) * p0.x + (sqrt(r1)*(1-sqrt(r2))) * p1.x + sqrt(r1)*r2*p2.x;
  y = (1-sqrt(r1)) * p0.y + (sqrt(r1)*(1-sqrt(r2))) * p1.y + sqrt(r1)*r2*p2.y;

  Point P = Point(x,y);
//  std::cerr << "random point inside c.h.: " << P << std::endl;

  // now I can convert the points to angles and distances from point P.
  // It's best to split the points into the 4 different quadrants.
  std::vector<Point> q1, q2, q3, q4;

  // shift the coordinates so the points in 'points' have coordinates relative to the point 'P'
  // and put them into the proper quadrant.
  for (unsigned int i=0; i<points.size();++i) {
    x = points[i].x - P.x;
    y = points[i].y - P.y;
    if (x < 0) {
      if (y < 0) q3.push_back(Point(x,y,points[i].i));
      else q2.push_back(Point(x,y,points[i].i));
    }
    else {
      if (y < 0) q4.push_back(Point(x,y,points[i].i));
      else q1.push_back(Point(x,y,points[i].i));
    }
  }

  std::sort(q1.begin(), q1.end(), angleComparator);
//  std::cerr << "q1: " << std:: endl;
//  pdisplay(q1);

  std::sort(q2.begin(), q2.end(), angleComparator);
//  std::cerr << "q2: " << std:: endl;
//  pdisplay(q2);

  std::sort(q3.begin(), q3.end(), angleComparator);
//  std::cerr << "q3: " << std:: endl;
//  pdisplay(q3);

  std::sort(q4.begin(), q4.end(), angleComparator);
//  std::cerr << "q4: " << std:: endl;
//  pdisplay(q4);

  // then push the points to the star vector, q2 needs to be reversed
  for (unsigned int i=0; i < q1.size();++i) {
    polygon.push_back(q1[i].i);
  }
  //for (auto it = q2.rbegin(); it != q2.rend(); ++it) {
  // polygon.push_back((*it).i);
  //}
  for (unsigned int i=0; i < q2.size();++i) {
    polygon.push_back(q2[i].i);
  }
  for (unsigned int i=0; i < q3.size();++i) {
    polygon.push_back(q3[i].i);
  }
  //for (auto it = q4.rbegin(); it != q4.rend(); ++it) {
  // polygon.push_back((*it).i);
  //}
  for (unsigned int i=0; i < q4.size();++i) {
    polygon.push_back(q4[i].i);
  }

//  std::cerr << "star:" << std::endl;
//  pdisplay(polygon, points);
  return retval;
}
