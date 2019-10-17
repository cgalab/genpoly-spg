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
#include "opt2e.h"
#include "opt2f.h"
#include "opt2g.h"
#include "curve.h"
#include "simple_pol_check.h"
#include "star.h"
#include "predicates.h"

int main(int argc, char *argv[]) {
  std::cout << std::setprecision(20);
  std::cerr << std::setprecision(20);
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
  bool writeNew = false; // whether to write a new file instead of saving over an existing file.
  bool calcArea = false;  // calculate and return the area
  bool calcCircumference = false; // calculate and return the circumference.
  bool checkSimple = false; // only verify a given point set and polygon is simple.
  //bool shew_pred = false; // enable shewchucks' predicates
  unsigned int randseed = 0;
  unsigned int nr_holes = 0;

  // parse command line arguments
  returnValue = argInit(argc, argv, inFile, outFile, &alg, &inFormat, &outFormat,
                        writeNew, calcArea, calcCircumference, randseed, checkSimple,
                        nr_holes, vFile);
//  std::cerr << "returnvalue: " << returnValue << std::endl;

  if (returnValue == SUCCESS) {
    // initialize Shewchuks' predicates
    exactinit();
    //std::cout << "all good to go" << std::endl;

    // points from input file saved in a vector
    std::vector<Point> points;
    // 'polygon' is an unsigned integer list of indexes into vector 'points'
    std::vector<unsigned int> polygon;
    // 'sph' is a vector of polygons, [0] is the simple polygon, subsequent polygons are holes in it.
    std::vector<std::vector<unsigned int>> sph;

    returnValue = readInFile(inFile, inFormat, points);
    if(returnValue == SUCCESS) {
      // verify that the point set is valid, i.e. no points are equal and the whole set isn't collinear.
      returnValue = verify_point_set(points);
      if(returnValue == SUCCESS) {
//        std::cerr << "point set verified." << std::endl;
        // get a simple polygon with a given method
        if (alg == A_2OPT) {
          returnValue = opt2(polygon, points, randseed);
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
        else if (alg == A_2OPT_E) {
          returnValue = opt2e(polygon, points, randseed);
        }
        else if (alg == A_2OPT_F) {
          returnValue = opt2f(polygon, points, randseed);
        }
        else if (alg == A_2OPT_G) {
          returnValue = opt2g(polygon, points, randseed);
        }
        else if (alg == A_STAR) {
          returnValue = star(polygon, points, randseed);
        }
        else if (alg == A_HOLE) {

          returnValue = holes2(sph, points, randseed, nr_holes);
          //returnValue = opt2(polygon, points, randseed);
          //returnValue = holes(sph, polygon, points, nr_holes);
        }

        if (returnValue == SUCCESS) {
          if (checkSimple) checkAllIntersections(polygon, points);
          if (calcArea) {
            double area = pol_calc_area(polygon, points);
            if(area < 0) {
              doFlip(0, polygon.size()-1, polygon, points);
              area = pol_calc_area(polygon, points);
            }
            std::cout << "Area: " << area << std::endl;
          }
          if (calcCircumference) std::cout << "Circumference: " << pol_calc_circumference(polygon, points) << std::endl;
          if (outFile[0] != 0) {
            switch(alg) {
              case A_2OPT:
              case A_2OPT_A:
              case A_2OPT_B:
              case A_2OPT_C:
              case A_2OPT_D:
              case A_2OPT_E:
              case A_2OPT_F:
              case A_2OPT_G:
              case A_STAR:
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
        if (calcArea) std::cout << "Area: " << pol_calc_area(polygon, points) << std::endl;
        if (calcCircumference) std::cout << "Circumference: " << pol_calc_circumference(polygon, points) << std::endl;
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
  else if (returnValue == RUN_LONG_CHECK) {
//    std::cerr << "running verify check" << std::endl;
    std::vector<Point> points;
    std::vector<unsigned int> polygon;
    returnValue = readInFile(inFile, inFormat, points);
    if (returnValue == SUCCESS) {
//      std::cerr << "points found in file:" << std::endl;
//      pdisplay(points);
      returnValue = readvFile(vFile, polygon, points);
      if (returnValue == SUCCESS) {
        if (calcArea) std::cout << "Area: " << pol_calc_area(polygon, points) << std::endl;
        if (calcCircumference) std::cout << "Circumference: " << pol_calc_circumference(polygon, points) << std::endl;
//        std::cerr << "indices found in file: " << std::endl;
//        pdisplay(polygon, points);
        if (points.size() == polygon.size()) {
//          std::cerr << "Nr. of points in both files match." << std::endl;
          if(checkAllIntersections(polygon, points)) returnValue = INTERSECTING_POINTS;
        }
        else std::cerr << "Error: Number of points mismatch between points file and polygon file." << std::endl;
      }
      else std::cerr << "Error reading polygon indices file." << std::endl;
    }
    else std::cerr << "Error reading points input file." << std::endl;
  }
  else if (returnValue == CONVERT_FORMAT) {
    // points from input file saved in a vector if 'Point' class
    std::vector<Point> points;
    // 'polygon' is an unsigned integer list of indexes into vector 'points'
    std::vector<unsigned int> polygon;

    returnValue = readInFile(inFile, inFormat, points);

    if (returnValue == SUCCESS) {
      // as we are only writing the points to another format, only a lexicogrpahical indes is needed.
      for (unsigned int i=0; i < points.size(); ++i) polygon.push_back(i);
      returnValue = writeOutFile(outFile, outFormat, writeNew, polygon, points);
    }
    else std::cerr << "Error reading infile!" << std::endl;

  }
  else if (returnValue == RUN_TESTS) test();
  return returnValue;
}
