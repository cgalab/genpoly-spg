#ifndef __ALLSP_H_
#define __ALLSP_H_

enum error allsp(std::vector<Point>& points);
void swap2 (std::vector<unsigned int>& a, int i, int j, bool& intersects, std::vector<Point>& points);
enum error heap2(std::vector<unsigned int>& a, int n, bool& intersects, std::vector<Point>& points);

#endif
