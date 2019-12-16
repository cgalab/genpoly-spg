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
#include "rand.h"
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
  unsigned int select_polygon = 0;

  // parse command line arguments
  returnValue = argInit(argc, argv, inFile, outFile, &alg, &inFormat, &outFormat,
                        writeNew, calcArea, calcCircumference, randseed, checkSimple,
                        nr_holes, select_polygon, vFile, run_tests, help);
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
  if (randseed) mt.seed(randseed);
  // initialize Shewchuks' predicates
  //exactinit();

  // points from input file saved in a vector
  std::vector<Point> points;
  // 'sph' is a vector of polygons, [0] is a polygon of points in 'points' containing
  // at minimum the convex hull points, subsequent polygons are holes in it.
  std::vector<std::vector<unsigned int>> sph;
  returnValue = readInFile(inFile, inFormat, points, sph);
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

  if (vFile[0] != 0) {
    returnValue = readvFile(vFile, sph[0], points);
    if (returnValue != SUCCESS) {
      std::cerr << "Error reading file with polygon!" << std::endl;
      return returnValue;
    }
    if (points.size() != sph[0].size()) {
      std::cerr << "Error: Number of points mismatch between points file and polygon file." << std::endl;
      return SIZE_NOT_EQUAL;
    }
    returnValue = simple_pol_check(sph[0], points);
    if (returnValue != SUCCESS) return returnValue;
  }

// std::cerr << "point set verified." << std::endl;
  // get a simple polygon with a given method
  if (alg == A_2OPT) {
    returnValue = opt2(sph[select_polygon], points);
  }
  else if (alg == A_2OPT_A) {
    returnValue = opt2a(sph[select_polygon], points);
  }
  else if (alg == A_2OPT_B) {
    returnValue = opt2b(sph[select_polygon], points);
  }
  else if (alg == A_2OPT_C) {
    returnValue = opt2c(sph[select_polygon], points);
  }
  else if (alg == A_2OPT_D) {
    returnValue = opt2d(sph[select_polygon], points);
  }
  else if (alg == A_2OPT_E) {
    returnValue = opt2e(sph[select_polygon], points);
  }
  else if (alg == A_2OPT_F) {
    returnValue = opt2f(sph[select_polygon], points);
  }
  else if (alg == A_2OPT_G) {
    returnValue = opt2g(sph[select_polygon], points);
  }
  else if (alg == A_STAR) {
    returnValue = star(sph[select_polygon], points);
  }
  else if (alg == A_HOLE) {
    returnValue = holes2(sph, points, sph[select_polygon], nr_holes);
  }
  else if (alg == A_ALLSP) {
    returnValue = allsp(points, outFile, outFormat, writeNew);
  }
  else if (alg == A_VERIFY) {
    returnValue = simple_pol_check(sph[select_polygon], points);
  }
  else if (alg == A_VERIFY_LONG) {
    if(checkAllIntersections(sph[select_polygon], points)) returnValue = INTERSECTING_POINTS;
  }
  if (returnValue != SUCCESS) {
    std::cerr << "Error running the algorithm!" << std::endl;
    return returnValue;
  }
  //if (checkSimple) checkAllIntersections(sph[select_polygon], points);
  if (calcArea) {
    double area = pol_calc_area(sph[select_polygon], points);
    if(area < 0) {
      doFlip(0, sph[select_polygon].size()-1, sph[select_polygon], points);
      area = pol_calc_area(sph[select_polygon], points);
    }
    std::cout << "Area: " << area << std::endl;
  }
  if (calcCircumference) std::cout << "Circumference: " << pol_calc_circumference(sph[select_polygon], points) << std::endl;
  // writing to outfile
  if (outFile[0] == 0) {
    std::cerr << "No outfile entered." << std::endl;
    std::cerr << "points:" << std::endl;
    pdisplay(points);
    for (unsigned int i = 0; i < sph.size(); ++i) {
      std::cerr << "polygon " << i << ":" << std::endl;
      pdisplay(sph[i], points);
    }
    return NO_OUT_FILE;
  }
  else {
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
      case A_HOLE:
      case A_VERIFY:
      case A_CONVERT_FORMAT:
        if (outFormat == OF_PURE_AND_PERM){
          char tempFileName[255];
          snprintf(tempFileName, sizeof(tempFileName), "%s%s", outFile, "-pure");
          returnValue = writeOutIntFile(tempFileName, OF_PURE, writeNew, sph[select_polygon], points);
          snprintf(tempFileName, sizeof(tempFileName), "%s%s", outFile, "-perm");
          if (returnValue == SUCCESS) returnValue = writeOutIntFile(tempFileName, OF_PERM, writeNew, sph[select_polygon], points);
        }
        else returnValue = writeOutFile(outFile, outFormat, writeNew, sph, points);
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
