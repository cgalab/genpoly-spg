#ifndef __INITS_H_
#define __INITS_H_

#include "point.h"

//int initArgs(int argc, char *argv[], char *inFile, char *outFile, enum alg_t *alg, enum out_format_t *outFormat, enum in_format_t *inFormat);
int algInit(enum alg_t *alg, char * optarg);

void ifInit(enum in_format_t *inFormat, char *optarg);

void ofInit(enum out_format_t *outFormat, char *optarg);

enum error argInit(int argc, char *argv[],
                   char *inFile, char *outFile,
                   enum alg_t *alg,
                   enum in_format_t *inFormat, enum out_format_t *outFormat,
                   bool& writeNew,
                   bool& area, double& areaMin, double& areaMax,
                   unsigned int& randseed, bool& checkSimple);

#endif
