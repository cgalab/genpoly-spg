#include <vector>
#include <stdio.h>
#include <string.h>
#include "basicDefinitions.h"
#include "point.h"


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
          std::cout << "Warning: unidentified input file format. Use -? for help." << std::endl;
          returnValue = READ_ERROR_IFORMAT;
          break;
      }

      ++counter;
    }


  }
  else if (fin == NULL) {
    returnValue = READ_ERROR_IFILE;
    std::cout << "Error: unable to open input file.  Use -? for help.: " << ((inFile[0] == 0) ? "No input given" : inFile)  << std::endl;
  }
  else if (inFormat == IF_UNDEFINED) {
    returnValue = READ_ERROR_IFORMAT;
    std::cout << "Error: input file format not defined. Use -? for help." << std::endl;
  }
  else {
    returnValue = UNEXPECTED_ERROR;
    std::cout << "Error:  unknown error opening input file." << std::endl;
  }
  return returnValue;
}


enum error writeOutFile(char *outFile, out_format_t outFormat, bool writeNew, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
  FILE *fout;

  if(writeNew) {
    fout = fopen(outFile, "r");

    if(fout != NULL) {
      //fprintf(stderr, "outFile already exists, need to create a new file\n");

      int counter = 0;
      char tempOutFile[255];

      do {
        snprintf(tempOutFile, sizeof(tempOutFile), "%s%d", outFile, counter);
        fout = fopen(tempOutFile, "r");
        ++counter;
      } while (fout != NULL);
          strcpy(outFile, tempOutFile);
    }
  }
  
  fout = fopen(outFile, "w");

  switch(outFormat) {
    case OF_PERM:
      for (unsigned int i = 0; i < polygon.size(); ++i)
        fprintf(fout, "%u\n", polygon[i]);
      break;
    case OF_POLY:
      fprintf(fout, "%lf %lf %lf %lf\n", getXmin(points), getXmax(points), getYmin(points), getYmax(points));
      fprintf(fout, "%lu\n", points.size());
      for (unsigned int i = 0; i < points.size(); ++i)
        fprintf(fout, "%lf %lf\n", points[i].x, points[i].y);
      break;
    case OF_DAT:
      fprintf(fout, "# X   Y\n");
      for (unsigned int i = 0; i < points.size(); ++i)
        fprintf(fout, "  %lf   %lf\n", points[polygon[i]].x, points[polygon[i]].y);
      fprintf(fout, "  %lf   %lf\n", points[polygon[0]].x, points[polygon[0]].y);
      break;
    case OF_UNDEFINED:
        std::cerr << "output format undefined.  Use -? for help." << std::endl;
      break;
    default:
      std::cerr << "unknown error writing output" << std::endl;
      break;
  }

  fclose(fout);
  return SUCCESS;
}