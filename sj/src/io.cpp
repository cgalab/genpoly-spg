#include <vector>
#include <stdio.h>
#include <string.h>
//#include <bitset>
#include "basicDefinitions.h"
#include "point.h"


enum error readInFile(char *inFile, in_format_t inFormat, std::vector<Point>& points, std::vector<std::vector<unsigned int>>& sph) {
  enum error returnValue = SUCCESS;

  FILE *fin = fopen(inFile, "r");
  if ((fin != NULL) && (inFormat != IF_UNDEFINED)) {
    char aLine[256];
    double x, y, xmin, xmax, ymin, ymax;
    unsigned int i=0, nrOfPoints=0, counter=0, sph_index = 0, points_index = 0, v_index = 0;
    int return_value;
    bool points_flag = false;
    Point p;
    if (sph.size() == 0) sph.push_back(std::vector<unsigned int>());

    while (fgets(aLine, sizeof(aLine), fin) != NULL) {
      if(aLine[0] == '#') continue; // ignore lines beginning with #
      if(aLine[0] == '\n') continue;
/*
      std::cerr.precision(24);
      std::cerr << "x: " << x << ", y: " << y << std::endl;
      std::cerr << "bitset version: " << std::endl;
      std::cerr << "x: " << std::bitset<sizeof(double) * 8> (*(reinterpret_cast<unsigned long long*>(&x))) << ", y: " << std::bitset<sizeof(double) * 8> (*(reinterpret_cast<unsigned long long*>(&y))) << std::endl;
      x = 0.0;
      y = 0.0;
      std::cerr << "x: " << x << ", y: " << y << std::endl;
      std::cerr << "bitset version: " << std::endl;
      std::cerr << "x: " << std::bitset<sizeof(double) * 8>(*(reinterpret_cast<unsigned long long*>(&x))) << ", y: " << std::bitset<sizeof(double) * 8>(*(reinterpret_cast<unsigned long long*>(&y))) << std::endl;
*/
      // Should I do regex matching instead of this ?
      // It would catch any errors when the format of the file doesn't match the command line argument for inFormat
      switch (inFormat) {
        case IF_POLY: // only accepts a single polygon
          if (counter == 0) {
            sscanf(aLine,"%lf %lf %lf %lf",&xmin,&xmax,&ymin,&ymax);
          }
          else if (counter == 1) {
            sscanf(aLine,"%u",&nrOfPoints);
          }
          else {
            sscanf(aLine,"%lf %lf",&x,&y);
            p.set(x,y,i,i,sph_index);
            points.push_back(p);
            sph[sph_index].push_back(i);
            ++i;
          }
          break;
        case IF_COMP:    // each line has "i x y" on it, only accepts a single polygon
          sscanf(aLine,"%u %lf %lf",&i,&x,&y);
          p.set(x,y,i,i,sph_index);
          points.push_back(p);
          sph[sph_index].push_back(i);
          break;
        case IF_POINTS:  // each line has only "x y" on it
        case IF_DAT:
        case IF_LINE:    // each polygon has a header which can be ignored
          return_value = sscanf(aLine,"%lf %lf",&x,&y);
          p.set(x,y,i,v_index,0,sph_index);
//          std::cerr << "io: p: " << p << std::endl;
          if (return_value == 2) {
            if (i == 0) {
              points.push_back(p);
              if (sph.size() == sph_index) sph.push_back(std::vector<unsigned int>());
              sph[sph_index].push_back(i);
              ++i;
              ++v_index;
            }
            else if (p != points[points_index]) {
              if (points_flag) {
                points_index = points.size();
                points_flag = false;
              }
              points.push_back(p);
              if (sph.size() == sph_index) sph.push_back(std::vector<unsigned int>());
              sph[sph_index].push_back(i);
              ++i;
              ++v_index;
            }
            else if (p == points[points_index]) {
              points_flag = true;
              ++sph_index;
              v_index = 0;
            }
          }
          break;
        case IF_UNDEFINED:
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

// Function to handle '-p <string>' argument which imports the polygons from the filename in <string> to 'sph'
enum error readvFile(char *vFile, std::vector<std::vector<unsigned int>>& sph, std::vector<Point>& points) {
  enum error returnValue = SUCCESS;
  unsigned int sph_index = 0;
  if (sph.size() != 0) {
    sph.resize(0);
  }

  FILE *fin = fopen(vFile, "r");
  if (fin != NULL) {
    char aLine[256];
    unsigned int i=0, counter=0;

    while (fgets(aLine, sizeof(aLine), fin) != NULL) {
      if(aLine[0] == '#') continue; // ignore lines beginning with #
      if(aLine[0] == '\n') continue; // ignore lines beginning with \newline

      sscanf(aLine,"%u",&i);
      if (counter == 0) {
//        std::cerr << "create new polygon: " << sph_index << ", at index: " << i << std::endl;
        sph.push_back(std::vector<unsigned int>());
        sph[sph_index].push_back(i);
        points[i].v = counter;
        points[i].p = sph_index;
        ++counter;
      }
      else {
        if (i == sph[sph_index][0]) {
//          std::cerr << "found an end: " << i << std::endl;
          ++sph_index;
          counter = 0;
        }
        else {
//          std::cerr << "adding: " << i << " into sph[" << sph_index << "]" << std::endl;
          sph[sph_index].push_back(i);
          points[i].v = counter;
          points[i].p = sph_index;
          ++counter;
        }
      }
    }
  }
  else if (fin == NULL) {
    returnValue = READ_ERROR_IFILE;
    std::cout << "Error: unable to open 'verify' file.  Use -? for help.: " << ((vFile[0] == 0) ? "No input given" : vFile)  << std::endl;
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
//      fprintf(stderr, "outFile already exists, need to create a new file\n");

      int counter = 0;
      char tempOutFile[255];

      do {
        snprintf(tempOutFile, sizeof(tempOutFile), "%s%d", outFile, counter);
//        std::cerr << tempOutFile << std::endl;
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
      fprintf(fout, "%.17f %.17f %.17f %.17f\n", getXmin(points), getXmax(points), getYmin(points), getYmax(points));
      fprintf(fout, "%lu\n", points.size());
      for (unsigned int i = 0; i < points.size(); ++i)
        fprintf(fout, "%.17f %.17f\n", points[i].x, points[i].y);
      break;
    case OF_DAT:
      fprintf(fout, "# X   Y\n");
      for (unsigned int i = 0; i < points.size(); ++i)
        fprintf(fout, "  %.17f   %.17f\n", points[polygon[i]].x, points[polygon[i]].y);
      fprintf(fout, "  %.17f   %.17f\n", points[polygon[0]].x, points[polygon[0]].y);
      break;
    case OF_LINE:
      fprintf(fout, "%lu\n", polygon.size());
      for (unsigned int i = 0; i < polygon.size(); ++i)
        fprintf(fout, "  %.17f   %.17f\n", points[polygon[i]].x, points[polygon[i]].y);
      fprintf(fout, "  %.17f   %.17f\n", points[polygon[0]].x, points[polygon[0]].y);
      break;
    case OF_PURE:
      for (unsigned int i = 0; i < points.size(); ++i)
        fprintf(fout, "%.17f %.17f\n", points[i].x, points[i].y);
      break;
    case OF_PURE_AND_PERM:
      std::cerr << "Should not be handled by this function." << std::endl;
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

// crappy function that assumes you are working with integer input values, and thus you can write them out as integers.
// Made to work with Peters' 'verify-it' code.
enum error writeOutIntFile(char *outFile, out_format_t outFormat, bool writeNew, std::vector<unsigned int>& polygon, std::vector<Point>& points) {
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
      fprintf(fout, "%.17f %.17f %.17f %.17f\n", getXmin(points), getXmax(points), getYmin(points), getYmax(points));
      fprintf(fout, "%lu\n", points.size());
      for (unsigned int i = 0; i < points.size(); ++i)
        fprintf(fout, "%.17f %.17f\n", points[i].x, points[i].y);
      break;
    case OF_DAT:
      fprintf(fout, "# X   Y\n");
      for (unsigned int i = 0; i < points.size(); ++i)
        fprintf(fout, "  %.17f   %.17f\n", points[polygon[i]].x, points[polygon[i]].y);
      fprintf(fout, "  %.17f   %.17f\n", points[polygon[0]].x, points[polygon[0]].y);
      break;
    case OF_PURE:
      for (unsigned int i = 0; i < points.size(); ++i)
        fprintf(fout, "%u %u\n", (unsigned int)points[i].x, (unsigned int)points[i].y);
      break;
    case OF_PURE_AND_PERM:
      std::cerr << "Should not be handled by this function." << std::endl;
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

enum error writeOutFile(char *outFile, out_format_t outFormat, bool writeNew, std::vector< std::vector<unsigned int> >& sph, std::vector<Point>& points) {
  enum error returnValue = SUCCESS;
  FILE *fout = NULL;
  bool writeToFile = false;

  if (outFile[0] != 0) {
    writeToFile = true;
    if(writeNew) { // need to check if the file already exists
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
  }

  for (unsigned int j = 0; j < sph.size(); ++j) {
    if (sph[j].size() == 0) continue;
    switch(outFormat) {
      case OF_PERM:
        writeToFile ? fprintf(fout, "#Polygon: %u\n", j+1) : fprintf(stdout, "#Polygon: %u\n", j+1);
        for (unsigned int i = 0; i < sph[j].size(); ++i)
          writeToFile ? fprintf(fout, "%u\n", sph[j][i]) : fprintf(stdout, "%u\n", sph[j][i]);
        writeToFile ? fprintf(fout, "\n") : fprintf(stdout, "\n");
        break;
      case OF_POLY:
        if (j == 0) {
          writeToFile ? fprintf(fout, "%.17f %.17f %.17f %.17f\n", getXmin(points), getXmax(points), getYmin(points), getYmax(points)) : fprintf(stdout, "%.17f %.17f %.17f %.17f\n", getXmin(points), getXmax(points), getYmin(points), getYmax(points));
        }
        writeToFile ? fprintf(fout, "%lu\n", sph[j].size()) : fprintf(stdout, "%lu\n", sph[j].size());
        for (unsigned int i = 0; i < sph[j].size(); ++i)
          writeToFile ? fprintf(fout, "%.17f %.17f\n", points[sph[j][i]].x, points[sph[j][i]].y) : fprintf(stdout, "%.17f %.17f\n", points[sph[j][i]].x, points[sph[j][i]].y);
        writeToFile ? fprintf(fout, "\n") : fprintf(stdout, "\n");
        break;
      case OF_DAT:
        writeToFile ? fprintf(fout, "# (index %u)\n", j) : fprintf(stdout, "# (index %u)\n", j);
        writeToFile ? fprintf(fout, "# X   Y\n") : fprintf(stdout, "# X   Y\n");
        for (unsigned int i = 0; i < sph[j].size(); ++i)
          writeToFile ? fprintf(fout, "  %.17f   %.17f\n", points[sph[j][i]].x, points[sph[j][i]].y) : fprintf(stdout, "  %.17f   %.17f\n", points[sph[j][i]].x, points[sph[j][i]].y);
        writeToFile ? fprintf(fout, "  %.17f   %.17f\n", points[sph[j][0]].x, points[sph[j][0]].y) : fprintf(stdout, "  %.17f   %.17f\n", points[sph[j][0]].x, points[sph[j][0]].y);
        writeToFile ? fprintf(fout, "\n\n") : fprintf(stdout, "\n\n");
        break;
      case OF_LINE:
        writeToFile ? fprintf(fout, "%lu\n", sph[j].size()) : fprintf(stdout, "%lu\n", sph[j].size());
        for (unsigned int i = 0; i < sph[j].size(); ++i)
          writeToFile ? fprintf(fout, "  %.17f   %.17f\n", points[sph[j][i]].x, points[sph[j][i]].y) : fprintf(stdout, "  %.17f   %.17f\n", points[sph[j][i]].x, points[sph[j][i]].y);
        writeToFile ? fprintf(fout, "  %.17f   %.17f\n", points[sph[j][0]].x, points[sph[j][0]].y) : fprintf(stdout, "  %.17f   %.17f\n", points[sph[j][0]].x, points[sph[j][0]].y);
        break;
      case OF_PURE:
        for (unsigned int i = 0; i < sph[j].size(); ++i)
          writeToFile ? fprintf(fout, "%.17f %.17f\n", points[sph[j][i]].x, points[sph[j][i]].y) : fprintf(stdout, "%.17f %.17f\n", points[sph[j][i]].x, points[sph[j][i]].y);
        writeToFile ? fprintf(fout, "\n") : fprintf(stdout, "\n");
        break;
      case OF_PURE_AND_PERM:
        std::cerr << "Should not be handled by this function." << std::endl;
        returnValue = UNEXPECTED_ERROR;
        break;
      case OF_UNDEFINED:
        //std::cerr << "output format undefined.  Use -H for help." << std::endl;
        returnValue = READ_ERROR_OFORMAT;
        break;
      default:
        std::cerr << "unknown error writing output" << std::endl;
        returnValue = WRITE_ERROR;
        break;
    }
  }
  if (writeToFile) {
    fclose(fout);
  }
  return returnValue;
}
