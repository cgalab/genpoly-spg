/* SIMPLE POLYGON GENERATOR LIBRARY */
/* start of development: 2019.03.08 */
/* Author: Steinþór Jasonarson      */

#include <time.h> // for time measurements
#include <iostream> // for endl
#include "string.h" // for strcpy
#include "basicDefinitions.h"
#include "inits.h"

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

  returnValue = argInit(argc, argv, inFile, outFile, &alg, &inFormat, &outFormat);
  if (returnValue == SUCCESS) {
    std::cout << "all good to go" << std::endl;
  }



  //begin = clock();
  //end = clock();
  //elapsed = (end - begin);
  //std::cout << "elapsed: " << elapsed << std::endl;

  return returnValue;
}