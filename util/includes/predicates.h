#define REAL double                      /* float or double */

#ifndef __ORIENT_H_
#define __ORIENT_H_

typedef struct {
   REAL x;
   REAL y;
} point;                                  


void exactinit();
double orient2d(point pa, point pb, point pc);

#endif