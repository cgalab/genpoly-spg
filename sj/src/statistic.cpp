#include <map>
#include <cmath> // for atan2
#include <point.h>


// Function to check the slopes of the edges of a polygon
enum error polslopecount(std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  double slope;
  std::map<double, unsigned int> slope_counter;
  std::map<double, unsigned int>::iterator s_it;
  Point p1, p2;

  p1 = points[polygon[polygon.size()-1]];
  for (unsigned int i = 0; i < polygon.size(); ++i) {
    p2 = points[polygon[i]];
    if (p1 < p2) {
      slope = atan2(p2.y - p1.y, p2.x - p1.x);
    }
    else {
      slope = atan2(p1.y - p2.y, p1.x - p2.x);
    }
//    std::cout << "i: " << p2 << ", slope: " << slope << std::endl;

    s_it = slope_counter.find(slope);
    if (s_it != slope_counter.end()) {
//      std::cerr << "slope_counter[slope]: " << slope_counter[slope] << std::endl;
//      std::cerr << "(*s_it).second: " << (*s_it).second << std::endl;
      slope_counter[slope] = (*s_it).second +1;
    }
    else {
      slope_counter[slope] = 1;
    }
  }
  for (std::map<double, unsigned int>::iterator it=slope_counter.begin(); it!=slope_counter.end(); ++it)
  std::cout << it->first << " : " << it->second << '\n';
  return SUCCESS;
}

// Function to check the slopes of the pointset
enum error pntslopecount(std::vector<Point>& points) {
  double slope, determinant, distance;
  bool found_coll;
  std::map<double, unsigned int> slope_counter;
  std::map<double, unsigned int>::iterator s_it;
  Point p1, p2, p3;

  for (unsigned int i = 0; i < points.size(); ++i) {
    p1 = points[i];
    for (unsigned int j = i+1; j < points.size(); ++j) {
      p2 = points[j];

//      std::cout << "i: " << i << ", p1: " << p1 << ", j: " << j << ", p2: " << p2 << std::endl;

      // we don't want to count the collinear slopes.
      found_coll = false;
      for (unsigned int k = 0; k < points.size(); ++k) {
        if (k == i || k == j) continue;

        p3 = points[k];
        determinant = det(p1, p2, p3);
        if (determinant == 0) {
          distance = reldist(p1, p2, p3);
//          std::cout << "det=0, dist: " << distance << ", p3: " << p3 << std::endl;
          if (0 < distance && distance < 1 ) {
            found_coll = true;
            break;
          }
        }
      }
      if (found_coll) continue;

      if (p1 < p2) {
        slope = atan2(p2.y - p1.y, p2.x - p1.x);
      }
      else {
        slope = atan2(p1.y - p2.y, p1.x - p2.x);
      }
//      std::cout << "slope: " << slope << std::endl;

      s_it = slope_counter.find(slope);
      if (s_it != slope_counter.end()) {
//        std::cerr << "slope_counter[slope]: " << slope_counter[slope] << std::endl;
//        std::cerr << "(*s_it).second: " << (*s_it).second << std::endl;
        slope_counter[slope] = (*s_it).second +1;
      }
      else {
        slope_counter[slope] = 1;
      }
    }
  }
  for (std::map<double, unsigned int>::iterator it=slope_counter.begin(); it!=slope_counter.end(); ++it)
  std::cout << it->first << " : " << it->second << '\n';
  return SUCCESS;
}
