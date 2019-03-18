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

  // parse command line arguments
  returnValue = argInit(argc, argv, inFile, outFile, &alg, &inFormat, &outFormat, writeNew);
  if (returnValue == SUCCESS) {
    //std::cout << "all good to go" << std::endl;

    // points from input file saved in a vector
    // needs to be public so the lexComp function can access values for comparison
    std::vector<Point> points;

    returnValue = readInFile(inFile, inFormat, &points);
    if(returnValue == SUCCESS) {

      // a polygon is a list of indexes in 'points'
      std::vector<unsigned int> polygon;

      // get a simple polygon with a given method
      returnValue = getSP(polygon, points, alg);

      if (returnValue == SUCCESS) {
        returnValue = writeOutFile(outFile, outFormat, writeNew, polygon, points);
      }
    }
  }

  //begin = clock();
  //end = clock();
  //elapsed = (end - begin);
  //std::cout << "elapsed: " << elapsed << std::endl;

  return returnValue;
}