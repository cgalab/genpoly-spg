#ifndef __POL_H_
#define __POL_H_

double pol_calc_area(std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool is_2D(std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool is_2D(std::pair<I_Edge,I_Edge> par, std::vector<unsigned int>& polygon, std::vector<Point>& points);
void get_convex_hull(std::vector<unsigned int>& ch, std::vector<Point>& points, bool enforceCCWOrder=false);
void get_inner_points(std::vector<unsigned int>& ip, std::vector<unsigned int>& ch, std::vector<Point>& points);
void createRandPol(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed);
void createCHRandPol(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed);
unsigned int get_cyclic_difference(unsigned int a, unsigned int b, unsigned int cycle);
void get_inner_chains_to_ch(std::vector< std::pair<I_Edge,I_Edge> >& ends, std::vector<unsigned int>& ch, std::vector<unsigned int>& polygon, std::vector<Point>& points);
bool checkAllIntersections (std::vector<unsigned int>& polygon, std::vector<Point>& points);

#endif
