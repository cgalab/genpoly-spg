#ifndef __ALLSP_H_
#define __ALLSP_H_

enum error allsp(std::vector<Point>& points, char *outFile, out_format_t& outFormat, bool& writeNew);
void swap2 (std::vector<unsigned int>& a, int i, int j, bool& intersects, std::vector<Point>& points);
enum error heap2(std::vector<unsigned int>& polygon, int n, bool& intersects, std::vector<Point>& points, std::set<unsigned int>& seen, unsigned int& counter, char *outFile, out_format_t& outFormat, bool& writeNew);
#endif
