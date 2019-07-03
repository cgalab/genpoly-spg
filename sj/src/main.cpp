/* SIMPLE POLYGON GENERATOR LIBRARY */
/* start of development: 2019.03.08 */
/* Author: Steinþór Jasonarson      */

#include <time.h> // for time measurements
#include <iostream> // for endl
#include <vector>
#include <list>
#include "string.h" // for strcpy
#include "basicDefinitions.h"
#include "basicFunctions.h"
#include "inits.h"
#include "io.h"
#include "point.h"
#include "edge.h"
#include "pol.h"
#include "opt2.h"
#include "curve.h"

int main(int argc, char *argv[]) {

  // final return value
  enum error returnValue = SUCCESS;
  // time measurement variables
  //clock_t begin, end;
  //double elapsed; // to get time in seconds, divide elapsed with CLOCKS_PER_SEC

  // command line variables
  char inFile[255] = {0}, outFile[255] = {0};
  enum alg_t alg = A_UNDEFINED;
  enum in_format_t inFormat = IF_UNDEFINED;
  enum out_format_t outFormat = OF_UNDEFINED;
  bool writeNew = false;
  bool calcArea = false;
  bool checkSimple = false;
  double areaMin = -1, areaMax = -1;
  int runAreaLoopFor, areaLoopCounter;
  clock_t areaTimerStart, areaTimerEnd;
  double maxTime, areaTimerElapsed;
  unsigned int randseed = 0;
  unsigned int nr_holes = 0;

  // parse command line arguments
  returnValue = argInit(argc, argv, inFile, outFile, &alg, &inFormat, &outFormat,
                        writeNew, calcArea, areaMin, areaMax, randseed, checkSimple, nr_holes);

  if (returnValue == SUCCESS) {
    runAreaLoopFor = 1000;
    maxTime = 600;
    //std::cout << "all good to go" << std::endl;

    // points from input file saved in a vector
    // needs to be public so the lexComp function can access values for comparison
    std::vector<Point> points;

    returnValue = readInFile(inFile, inFormat, &points);
    if(returnValue == SUCCESS) {

      // a polygon is a list of indexes in 'points'
      std::vector<unsigned int> polygon;

      // get a simple polygon with a given method
      if (alg == A_2OPT) {
        if (calcArea)
        {
          double area = 0;
          areaLoopCounter = 0;
          areaTimerStart = clock();
          do
          {
            ++areaLoopCounter;

            // get a simple polygon with a given method
            returnValue = opt2(polygon, points, randseed);

            area = pol_calc_area(polygon, points);
            areaTimerEnd = clock();
            areaTimerElapsed = (areaTimerEnd - areaTimerStart) / CLOCKS_PER_SEC;

            //std::cerr << "areaMin: " << areaMin << ", areaMax: " << areaMax << ", area: " << area << std::endl;
            //std::cerr << "time elapsed: " << areaTimerElapsed << ", areaLoopCounter: " << areaLoopCounter << std::endl;

            if ((areaMin >= 0) && (area > 0) && (area > areaMin)) break;
            if ((areaMax > 0)  && (area > 0) && (area < areaMax)) break;
            if (randseed) break;
          } while ((areaLoopCounter < runAreaLoopFor) && (areaTimerElapsed < maxTime));


          if(area < 0) {
            doFlip(0, polygon.size()-1, polygon, points);
            area = pol_calc_area(polygon, points);
            std::cout << std::setprecision(15) << area << std::endl;
          }
          else if (((areaMin >= 0) && (area > areaMin)) || ((areaMax > 0) && (area < areaMax)))
            std::cout << std::setprecision(15) << area << std::endl;
          else
            returnValue = ERR_AREA_NOT_BETTER;
        }
        else {
          // get a simple polygon with a given method
          returnValue = opt2(polygon, points, randseed);
        }
      }
      else if (alg == A_HOLE) {
        if (nr_holes == 0) nr_holes = 1;
        // a vector of polygons, [0] is the simple polygon, subsequent polygons are holes in it.
        std::vector<std::vector<unsigned int>> sph;

        // get a simple polygon to work with.
        returnValue = opt2(polygon, points, randseed);
        if (returnValue == SUCCESS) {
           returnValue = holes2(sph, polygon, points, nr_holes);
        }
      }

      if (returnValue == SUCCESS) {
        if (checkSimple) checkAllIntersections(polygon, points);
        if (outFile[0] != 0) returnValue = writeOutFile(outFile, outFormat, writeNew, polygon, points);
      }
    }
  }
  else if (returnValue == RUN_TESTS) test();

  //begin = clock();
  //end = clock();
  //elapsed = (end - begin);
  //std::cout << "elapsed: " << elapsed << std::endl;

  return returnValue;
}
