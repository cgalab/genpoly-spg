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
#include "allsp.h"
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
  bool run_tests = false, help = false;
  //bool shew_pred = false; // enable shewchucks' predicates
  unsigned int randseed = 0;
  unsigned int nr_holes = 0;

  // parse command line arguments
  returnValue = argInit(argc, argv, inFile, outFile, &alg, &inFormat, &outFormat,
                        writeNew, calcArea, calcCircumference, randseed, checkSimple,
                        nr_holes, vFile, run_tests, help);
//  std::cerr << "returnvalue: " << returnValue << std::endl;

  if (help) return SUCCESS;
  if (returnValue != SUCCESS) {
    std::cerr << "Error in initialising arguments!" << std::endl;
    return returnValue;
  }
  if (run_tests) {
    test();
    return returnValue;
  }
  // initialize Shewchuks' predicates
  //exactinit();

  // points from input file saved in a vector
  std::vector<Point> points;
  returnValue = readInFile(inFile, inFormat, points);
  if (returnValue != SUCCESS) {
    std::cerr << "Error reading file with points!" << std::endl;
    return returnValue;
  }
  // verify that the point set is valid, i.e. no points are equal and the whole set isn't collinear.
  returnValue = verify_point_set(points);
  if(returnValue != SUCCESS) {
    std::cerr << "Error verifying point set!" << std::endl;
    return returnValue;
  }

  // 'polygon' is an unsigned integer list of indexes into vector 'points'
  std::vector<unsigned int> polygon;
  if (vFile[0] != 0) {
    returnValue = readvFile(vFile, polygon, points);
    if (returnValue != SUCCESS) {
      std::cerr << "Error reading file with polygon!" << std::endl;
      return returnValue;
    }
    if (points.size() != polygon.size()) {
      std::cerr << "Error: Number of points mismatch between points file and polygon file." << std::endl;
      return SIZE_NOT_EQUAL;
    }
    returnValue = simple_pol_check(polygon, points);
    if (returnValue != SUCCESS) return returnValue;
  }

  // 'sph' is a vector of polygons, [0] is the simple polygon, subsequent polygons are holes in it.
  std::vector<std::vector<unsigned int>> sph;

// std::cerr << "point set verified." << std::endl;
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
    returnValue = opt2d(polygon, points, randseed);
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
    returnValue = holes2(sph, points, polygon, randseed, nr_holes);
  }
  else if (alg == A_ALLSP) {
    returnValue = allsp(points, outFile, outFormat, writeNew);
  }
  else if (alg == A_VERIFY) {
    return simple_pol_check(polygon, points);
  }
  else if (alg == A_VERIFY_LONG) {
    if(checkAllIntersections(polygon, points)) return INTERSECTING_POINTS;
  }

  if (returnValue != SUCCESS) {
    std::cerr << "Error running the algorithm!" << std::endl;
    return returnValue;
  }
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
      if (outFile[0] == 0) {
        std::cerr << "Error!  No outfile entered!" << std::endl;
        return NO_OUT_FILE;
      }
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
    case A_CONVERT_FORMAT:
      // as we are only writing the points to another format, only a lexicographical index is needed.
      for (unsigned int i=0; i < points.size(); ++i) polygon.push_back(i);
      returnValue = writeOutFile(outFile, outFormat, writeNew, polygon, points);
      break;
    case A_CURVE:
    case A_IDLE:
    case A_UNDEFINED:
    default:
      returnValue = UNEXPECTED_ERROR;
      break;
  }
}
