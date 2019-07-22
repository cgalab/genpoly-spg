#ifndef __2OPT_H_
#define __2OPT_H_

#include <vector>
#include <utility> // for std::pair
#include <list>
#include "basicDefinitions.h"
#include "point.h"
#include "edge.h"

bool sortbysec(const std::pair<Point,double> &a, const std::pair<Point,double> &b);
bool sortbyint(const int i, const int j);
enum edge_t edgeCheck(unsigned int& index, Edge& e1, std::list<Edge>& edges, std::vector<unsigned int>& polygon, std::vector<Point>& points);
enum error opt2(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed);

#endif
