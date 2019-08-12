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
#include "opt2a.h"
#include "opt2b.h"
#include "opt2c.h"
#include "opt2d.h"
#include "curve.h"
#include "simple_pol_check.h"

int main(int argc, char *argv[]) {

  // final return value
  enum error returnValue = SUCCESS;
  // time measurement variables
  //clock_t begin, end;
  //double elapsed; // to get time in seconds, divide elapsed with CLOCKS_PER_SEC

  // command line variables
  char inFile[255] = {0}, outFile[255] = {0}, vFile[255] = {0};
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
                        writeNew, calcArea, areaMin, areaMax, randseed, checkSimple,
                        nr_holes, vFile);
//  std::cerr << "returnvalue: " << returnValue << std::endl;

  if (returnValue == SUCCESS) {
    //std::cout << "all good to go" << std::endl;

    // points from input file saved in a vector
    // needs to be public so the lexComp function can access values for comparison
    std::vector<Point> points;
    // 'polygon' is an unsigned integer list of indexes into vector 'points'
    std::vector<unsigned int> polygon;
    // 'sph' is a vector of polygons, [0] is the simple polygon, subsequent polygons are holes in it.
    std::vector<std::vector<unsigned int>> sph;

    returnValue = readInFile(inFile, inFormat, points);
    if(returnValue == SUCCESS) {
      // get a simple polygon with a given method
      if (alg == A_2OPT) {
        if (calcArea)
        {
          runAreaLoopFor = 1000;
          maxTime = 600;
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
      else if (alg == A_2OPT_A) {
        returnValue = opt2a(polygon, points, randseed);
      }
      else if (alg == A_2OPT_B) {
        returnValue = opt2b(polygon, points, randseed);
      }
      else if (alg == A_2OPT_C) {
          returnValue = opt2c(polygon, points, randseed);
      }
      else if (alg == A_2OPT_D) {
          returnValue = opt2c(polygon, points, randseed);
      }
      else if (alg == A_HOLE) {

        returnValue = holes2(sph, points, randseed, nr_holes);
        //returnValue = opt2(polygon, points, randseed);
        //returnValue = holes(sph, polygon, points, nr_holes);
      }

      if (returnValue == SUCCESS) {
        if (checkSimple) checkAllIntersections(polygon, points);
        if (outFile[0] != 0) {
          switch(alg) {
            case A_2OPT:
            case A_2OPT_A:
            case A_2OPT_B:
            case A_2OPT_C:
            case A_2OPT_D:
              if (outFormat == OF_PURE_AND_PERM){
                char tempFileName[255];
                snprintf(tempFileName, sizeof(tempFileName), "%s%s", outFile, "-pure");
                returnValue = writeOutIntFile(tempFileName, OF_PURE, writeNew, polygon, points);
                snprintf(tempFileName, sizeof(tempFileName), "%s%s", outFile, "-perm");
                if (returnValue == SUCCESS) returnValue = writeOutIntFile(tempFileName, OF_PERM, writeNew, polygon, points);
              }
              else returnValue = writeOutFile(outFile, outFormat, writeNew, polygon, points);
              break;
            case A_HOLE:
              returnValue = writeOutFile(outFile, outFormat, writeNew, sph, points);
              break;
            case A_CURVE:
            case A_IDLE:
            case A_UNDEFINED:
            default:
              returnValue = UNEXPECTED_ERROR;
              break;
          }

        }
      }
    }
  }
  // '--verify' command works alone, so if the verify file is set, only run that.
  else if (returnValue == RUN_SIMPLE_CHECK) {
//    std::cerr << "running verify check" << std::endl;
    std::vector<Point> points;
    std::vector<unsigned int> polygon;
    returnValue = readInFile(inFile, inFormat, points);
    if (returnValue == SUCCESS) {
//      std::cerr << "points found in file:" << std::endl;
//      pdisplay(points);
      returnValue = readvFile(vFile, polygon, points);
      if (returnValue == SUCCESS) {
//        std::cerr << "indices found in file: " << std::endl;
//        pdisplay(polygon, points);
        if (points.size() == polygon.size()) {
//          std::cerr << "Nr. of points in both files match." << std::endl;
          returnValue = simple_pol_check(polygon, points);
        }
        else std::cerr << "Error: Number of points mismatch between points file and polygon file." << std::endl;
      }
      else std::cerr << "Error reading polygon indices file." << std::endl;
    }
    else std::cerr << "Error reading points input file." << std::endl;
  }
  else if (returnValue == RUN_TESTS) test();

  //begin = clock();
  //end = clock();
  //elapsed = (end - begin);
  //std::cout << "elapsed: " << elapsed << std::endl;

  return returnValue;
}
