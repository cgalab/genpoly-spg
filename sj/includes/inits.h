#ifndef __INITS_H_
#define __INITS_H_

#include "point.h"

enum error inFileInit(char *inFile, char *optarg);
enum error outFileInit(char *outFile, char *optarg);
//int initArgs(int argc, char *argv[], char *inFile, char *outFile, enum alg_t *alg, enum out_format_t *outFormat, enum in_format_t *inFormat);
enum error algInit(enum alg_t *alg, char *optarg);

enum error ifInit(enum in_format_t *inFormat, char *optarg);

enum error ofInit(enum out_format_t *outFormat, char *optarg);

enum error argInit(	int argc, char *argv[],
										char *inFile, char *outFile, enum alg_t *alg,
										enum in_format_t *inFormat, enum out_format_t *outFormat,
										bool& writeNew, bool& area,	bool& area_ratio, bool& circumference, bool& norm_perimeter,
										unsigned int& randseed, bool& checkSimple, bool& generate_holes, unsigned int& holes,
										unsigned int& selected_polygon, char *vFile, bool& run_tests, bool& help);

#endif
