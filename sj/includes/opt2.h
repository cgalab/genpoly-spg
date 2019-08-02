#ifndef __2OPT_H_
#define __2OPT_H_

#include <vector>
#include <utility> // for std::pair
#include <list>
#include "basicDefinitions.h"
#include "point.h"
#include "edge.h"

enum edge_t removeEdgeFromSet(Edge& e, std::set<Edge>& edgeS, std::vector<unsigned int>& polygon, std::vector<Point>& points);
enum error opt2(std::vector<unsigned int>& polygon, std::vector<Point>& points, unsigned int randseed);

#endif
