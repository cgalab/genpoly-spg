#include <vector>
#include <stdio.h>
#include "basicDefinitions.h"
#include "points.h"


enum error readInFile(char *inFile,in_format_t inFormat, std::vector<Point> *points) {
  enum error returnValue = SUCCESS;

  FILE *fin = fopen(inFile, "r");
  if ((fin != NULL) && (inFormat != IF_UNDEFINED)) {
    char aLine[256];
    double x, y, xmin, xmax, ymin, ymax;
    unsigned int i=0, nrOfPoints=0, counter=0;
    Point p;

    while (fgets(aLine, sizeof(aLine), fin) != NULL) {
      if(aLine[0] == '#') continue; // ignore lines beginning with #

      // Should I do regex matching instead of this ?
      // It would catch any errors when the format of the file doesn't match the command line argument for inFormat
      switch (inFormat) {
        case IF_POINTS:  // each line has only "x y" on it
          sscanf(aLine,"%lf %lf",&x,&y);
          p.set(x,y,counter);
          (*points).push_back(p);
          break;
        case IF_POLY:
          if (counter == 0) {
            sscanf(aLine,"%lf %lf %lf %lf",&xmin,&xmax,&ymin,&ymax);
          }
          else if (counter == 1) {
            sscanf(aLine,"%u",&nrOfPoints);
          }
          else {
            sscanf(aLine,"%lf %lf",&x,&y);
            p.set(x,y,i);
            (*points).push_back(p);
            ++i;
          }
          break;
        case IF_COMP:    // each line has "i x y" on it
          sscanf(aLine,"%u %lf %lf",&i,&x,&y);
          p.set(x,y,i);
          (*points).push_back(p);
          break;
        default:
          std::cout << "Warning: unidentified input file format, " << std::endl;
          returnValue = READ_ERROR_IFORMAT;
          break;
      }

      ++counter;
    }


  }
  else if (inFormat == IF_UNDEFINED) {
    returnValue = READ_ERROR_IFORMAT;
    std::cout << "Error: input file format not defined." << std::endl;
  }
  else if (fin == NULL) {
    returnValue = READ_ERROR_IFILE;
    std::cout << "Error: unable to open input file: " << inFile << std::endl;
  }
  else {
    returnValue = UNEXPECTED_ERROR;
    std::cout << "Error:  unknown error opening input file" << std::endl;
  }
  return returnValue;
}