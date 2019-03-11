/* SIMPLE POLYGON GENERATOR LIBRARY */
/* start of development: 2019.03.08 */
/* Author: Steinþór Jasonarson      */

#include <time.h> // for time measurements
#include <iostream> // for endl
#include <vector>
#include "string.h" // for strcpy
#include "basicDefinitions.h"
#include "inits.h"
#include "points.h"
#include "io.h"

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

  // parse command line arguments
  returnValue = argInit(argc, argv, inFile, outFile, &alg, &inFormat, &outFormat);
  if (returnValue == SUCCESS) {
    //std::cout << "all good to go" << std::endl;

    std::vector<Point> points;

    returnValue = readInFile(inFile, inFormat, &points);
    if(returnValue == SUCCESS) {
      pdisplay(points);

      // points are inside a vector.  Now do some calculations on them
      


    }
  }

  //begin = clock();
  //end = clock();
  //elapsed = (end - begin);
  //std::cout << "elapsed: " << elapsed << std::endl;

  return returnValue;
}