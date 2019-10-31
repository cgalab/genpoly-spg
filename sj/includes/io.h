#ifndef __IO_H_
#define __IO_H_

enum error readInFile(char *inFile, in_format_t inFormat, std::vector<Point>& points);
enum error readvFile(char *vFile, std::vector<unsigned int>& polygon, std::vector<Point>& points);
enum error writeOutFile(char *outFile, out_format_t outFormat, bool writeNew, std::vector<unsigned int>& polygon, std::vector<Point>& points);
enum error writeOutIntFile(char *outFile, out_format_t outFormat, bool writeNew, std::vector<unsigned int>& polygon, std::vector<Point>& points);
enum error writeOutFile(char *outFile, out_format_t outFormat, bool writeNew, std::vector< std::vector<unsigned int> >& sph, std::vector<Point>& points);

#endif
