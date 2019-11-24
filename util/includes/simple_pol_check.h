#ifndef __SIMPLE_POL_CHECK_H_
#define __SIMPLE_POL_CHECK_H_

enum error simple_pol_check(std::vector<unsigned int>& polygon, std::vector<Point>& points);
std::pair<E_Edge, enum error> simple_pol_check2(std::vector<unsigned int>& polygon, std::vector<Point>& points);

#endif
