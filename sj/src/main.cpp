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
  double areaMin = -1, areaMax = -1;
  int runAreaLoopFor, areaLoopCounter;
  clock_t areaTimerStart, areaTimerEnd;
  double maxTime, areaTimerElapsed;
  unsigned int randseed = 0;

  // parse command line arguments
  returnValue = argInit(argc, argv, inFile, outFile, &alg, &inFormat, &outFormat, writeNew, calcArea, areaMin, areaMax, randseed);

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
      //returnValue = getSP(polygon, points, alg);

      if (calcArea)
      {
        double area = 0;
        areaLoopCounter = 0;
        areaTimerStart = clock();
        do
        {
          ++areaLoopCounter;

          // get a simple polygon with a given method
          returnValue = getSP(polygon, points, alg, randseed);

          area = pol_calc_area(polygon, points);
          areaTimerEnd = clock();
          areaTimerElapsed = (areaTimerEnd - areaTimerStart) / CLOCKS_PER_SEC;
          if ((areaMin >= 0) && (area > 0) && (area > areaMin)) break;
          if ((areaMax > 0) && (area > 0) && (area < areaMax)) break;
        } while ((areaLoopCounter < runAreaLoopFor) && (areaTimerElapsed < maxTime));
        //std::cerr << "areaMin: " << areaMin << ", areaMax: " << areaMax << ", area: " << area << std::endl;
        //std::cerr << "time elapsed: " << areaTimerElapsed << ", areaLoopCounter: " << areaLoopCounter << std::endl;

        if(area < 0)
          returnValue = ERR_AREA_NEGATIVE;
        else if (((areaMin >= 0) && (area > areaMin)) || ((areaMax > 0) && (area < areaMax)))
          std::cout << std::setprecision(15) << area << std::endl;
        else
          returnValue = ERR_AREA_NOT_BETTER;
      }
      else {
        // get a simple polygon with a given method
        returnValue = getSP(polygon, points, alg, randseed);
      }

      if (returnValue == SUCCESS) {
        //checkAllIntersections(polygon, points);
        returnValue = writeOutFile(outFile, outFormat, writeNew, polygon, points);
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
