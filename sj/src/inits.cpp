#include <iostream> // for endl
#include <getopt.h> // for parsing command line arguments
#include <stdio.h>
#include <string.h>
#include <vector>
#include "basicDefinitions.h"
#include "point.h"

/*
  Functions that deal with possible errors in the command line argument parsing
*/

enum error inFileInit(char *inFile, char *optarg) {
  enum error returnValue = SUCCESS;
  if(optarg[0] == 0) {
    std::cerr << "Error: no input entered for inFile" << std::endl;
    returnValue = NO_IN_FILE;
  }
  else if (optarg[0] == '-') {
    std::cerr << "Error: another command set as input <string> for --infile" << std::endl;
    returnValue = READ_ERROR_IFILE;
  }
  else {
    strcpy(inFile, optarg);
  }

  return returnValue;
}

enum error outFileInit(char *outFile, char *optarg) {
  enum error returnValue = SUCCESS;
  if(optarg[0] == 0) {
    std::cerr << "Error: no input entered for outFile" << std::endl;
    returnValue = NO_OUT_FILE;
  }
  else if (optarg[0] == '-') {
    std::cerr << "Error: another command set as input <string> for --outfile" << std::endl;
    returnValue = READ_ERROR_OFILE;
  }
  else {
    strcpy(outFile, optarg);
  }

  return returnValue;
}

enum error algInit(enum alg_t *alg, char *optarg) {
  enum error returnValue = SUCCESS;
  if(strcmp(optarg,"2opt") == 0) {
    *alg = A_2OPT;
  }
  else {
    *alg = A_UNDEFINED;
    std::cerr << "Error:  --alg input incorrect.  Input: '" << optarg << "', should be '2opt', 'space', or 'quick'." << std::endl;
    returnValue = NO_ALGORITHM;
  }
  return returnValue;
}

enum error ifInit(enum in_format_t *inFormat, char *optarg) {
  enum error returnValue = SUCCESS;
       if (strcmp(optarg,"points") == 0) *inFormat = IF_POINTS;
  else if (strcmp(optarg,"poly") == 0) *inFormat = IF_POLY;
  else if (strcmp(optarg,"comp") == 0) *inFormat = IF_COMP;
  else {
    *inFormat = IF_UNDEFINED;
    std::cerr << "Error:  --informat input incorrect.  Input: '" << optarg << "', should be 'points', 'json', or 'comp'." << std::endl;
    returnValue = READ_ERROR_IFORMAT;
  }
  return returnValue;
}

enum error ofInit(enum out_format_t *outFormat, char *optarg) {
  enum error returnValue = SUCCESS;
  if (strcmp(optarg,"perm") == 0) *outFormat = OF_PERM;
  else if (strcmp(optarg,"poly") == 0) *outFormat = OF_POLY;
  else {
    *outFormat = OF_UNDEFINED;
    std::cerr << "Error:  --outformat input incorrect.  Input: '" << optarg << "', should be 'perm', 'json', or 'poly'." << std::endl;
    returnValue = READ_ERROR_OFORMAT;
  }
  return returnValue;
}

enum error argInit(int argc, char *argv[], char *inFile, char *outFile, enum alg_t *alg, enum in_format_t *inFormat, enum out_format_t *outFormat, bool& writeNew) {
  enum error returnValue = SUCCESS;
  int comm;

  static struct option long_options[] =
  {
    {"infile", required_argument, NULL, 'i'},
    {"outfile", required_argument, NULL, 'o'},
    {"alg", required_argument, NULL, 'a'},
    {"informat", required_argument, NULL, 'b'},
    {"outformat", required_argument, NULL, 'c'},
    {"writeNew", no_argument, NULL, 'w'},
    {0, 0, 0, 0}
  };

  while( (comm = getopt_long (argc, argv, "i:o:a:b:c:w", long_options, NULL)) != -1 ) {
        
    switch(comm) {
      case 'i':
        returnValue = inFileInit(inFile, optarg);
        break;
      case 'o':
        returnValue = outFileInit(outFile, optarg);
        break;
      case 'a':
        returnValue = algInit(alg, optarg);
        break;
      case 'b':
        returnValue = ifInit(inFormat, optarg);
        break;
      case 'c':
        returnValue = ofInit(outFormat, optarg);
        break;
      case 'w':
        writeNew = true;
        break;

      default:
        std::cerr << "Error:  input arguments not formatted correctly." << std::endl;
        returnValue = UNEXPECTED_ERROR;
    }
  }

  if (optind < argc)
    {
      printf ("non-option ARGV-elements: ");
      while (optind < argc)
        printf ("%s ", argv[optind++]);
      putchar ('\n');
    }

  return returnValue;
}