#include <vector>
#include "basicDefinitions.h"
#include "point.h"

#ifndef __2OPT_H_
#define __2OPT_H_

bool sortbysec(const pair<Point,double> &a, const pair<Point,double> &b)
bool sortbyint(const int i, const int j)
void movePoints(std::vector<unsigned int>& moveP, std::vector<unsigned int>& polygon, std::vector<Point>& points);
enum edge_t edgeCheck(unsigned int& index, Edge& e1, std::list<Edge>& edges, std::vector<unsigned int>& polygon, std::vector<Point>& points);
enum error opt2(std::vector<unsigned int>& polygon, std::vector<Point>& points);

#endif