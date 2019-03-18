#include <vector>
#include <iostream> // for endl
#include "point.h"
#include <math.h>

// simple function to display points in a Point vector.
void pdisplay (const std::vector<Point>& p) {
  for (unsigned int i = 0; i < p.size(); i++) {
    std::cout << p[i] << std::endl;
  }
}

double getXmin(const std::vector<Point>& p) {
  double xmin = INFINITY;
  for (unsigned int i = 0; i < p.size(); i++) {
    if (p[i].x < xmin) xmin = p[i].x;
  }
  return xmin;
}

double getXmax(const std::vector<Point>& p) {
  double xmax = 0;
  for (unsigned int i = 0; i < p.size(); i++) {
    if (p[i].x > xmax) xmax = p[i].x;
  }
  return xmax;
}

double getYmin(const std::vector<Point>& p) {
  double ymin = INFINITY;
  for (unsigned int i = 0; i < p.size(); i++) {
    if (p[i].y < ymin) ymin = p[i].y;
  }
  return ymin;
}

double getYmax(const std::vector<Point>& p) {
  double ymax = 0;
  for (unsigned int i = 0; i < p.size(); i++) {
    if (p[i].y > ymax) ymax = p[i].y;
  }
  return ymax;
}