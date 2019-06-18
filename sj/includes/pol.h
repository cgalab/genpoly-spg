#ifndef __POL_H_
#define __POL_H_

double pol_calc_area(std::vector<unsigned int>& polygon, std::vector<Point>& points);
void get_convex_hull(std::vector<unsigned int>& ch, std::vector<Point>& points, bool enforceCCWOrder=false);
void createRandPol(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed);
void createCHRandPol(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed);
bool checkAllIntersections (std::vector<unsigned int>& polygon, std::vector<Point>& points);

#endif
