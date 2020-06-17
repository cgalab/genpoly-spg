#include <iostream> // for endl
#include <getopt.h> // for parsing command line arguments
#include <stdio.h>
#include <string.h>
#include <vector>
#include <stdlib.h>     /* atof */
#include <math.h> // INFINITY
#include "basicDefinitions.h"
#include "point.h"

/*
	Functions that deal with the command line argument parsing
*/

enum error inFileInit(char *inFile, char *optarg) {
	enum error returnValue = SUCCESS;
	if(optarg[0] == 0) {
		std::cerr << "Error: no input entered for inFile. Use -H for help." << std::endl;
		returnValue = NO_IN_FILE;
	}
	else if (optarg[0] == '-') {
		std::cerr << "Error: another command set as input <string> for --infile. Use -H for help." << std::endl;
		returnValue = READ_ERROR_IFILE;
	}
	else {
		strcpy(inFile, optarg);
	}

	return returnValue;
}

enum error outFileInit(char *outFile, char *optarg) {
	enum error returnValue = SUCCESS;
	/*
	if(optarg[0] == 0) {
		std::cerr << "Error: no input entered for outFile. Use -H for help." << std::endl;
		returnValue = NO_OUT_FILE;
	}
	else */
	if (optarg[0] == '-') {
		std::cerr << "Error: another command set as input <string> for --outfile. Use -H for help." << std::endl;
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
	else if(strcmp(optarg,"0") == 0) {
		*alg = A_2OPT_A;
	}
	else if(strcmp(optarg,"1") == 0) {
		*alg = A_2OPT_B;
	}
	else if(strcmp(optarg,"2") == 0) {
		*alg = A_2OPT_C;
	}
	else if(strcmp(optarg,"3") == 0) {
		*alg = A_2OPT_D;
	}
	else if(strcmp(optarg,"4") == 0) {
		*alg = A_2OPT_E;
	}
	else if(strcmp(optarg,"5") == 0) {
		*alg = A_2OPT_F;
	}
	else if(strcmp(optarg,"6") == 0) {
		*alg = A_2OPT_G;
	}
	else if(strcmp(optarg,"7") == 0) {
		*alg = A_2OPT_H;
	}
	else if(strcmp(optarg,"curve") == 0) {
		*alg = A_CURVE;
	}
	else if(strcmp(optarg,"star") == 0) {
		*alg = A_STAR;
	}
	else if(strcmp(optarg,"allsp") == 0) {
		*alg = A_ALLSP;
	}
	else if(strcmp(optarg,"verify") == 0) {
		*alg = A_VERIFY;
	}
	else if(strcmp(optarg,"verify_long") == 0) {
		*alg = A_VERIFY_LONG;
	}
	else if(strcmp(optarg,"polslope") == 0) {
		*alg = A_POLSLOPE;
	}
	else if(strcmp(optarg,"pntslope") == 0) {
		*alg = A_PNTSLOPE;
	}
	else if(strcmp(optarg,"convert") == 0) {
		*alg = A_CONVERT_FORMAT;
	}
	else {
		*alg = A_UNDEFINED;
		std::cerr << "Error:  --alg input incorrect.  Use -H for help. Input: '" << optarg << "', should be '2opt'." << std::endl;
		returnValue = NO_ALGORITHM;
	}
	return returnValue;
}

enum error ifInit(enum in_format_t *inFormat, char *optarg) {
	enum error returnValue = SUCCESS;
			 if (strcmp(optarg,"point") == 0) *inFormat = IF_POINTS;
	else if (strcmp(optarg,"pnt") == 0) *inFormat = IF_POINTS;
	else if (strcmp(optarg,"poly") == 0) *inFormat = IF_POLY;
	else if (strcmp(optarg,"pol") == 0) *inFormat = IF_POLY;
	else if (strcmp(optarg,"comp") == 0) *inFormat = IF_COMP;
	else if (strcmp(optarg,"line") == 0) *inFormat = IF_LINE;
	else if (strcmp(optarg,"dat") == 0) *inFormat = IF_DAT;
	else {
		*inFormat = IF_UNDEFINED;
		std::cerr << "Error:  --informat input incorrect.  Use -H for help. Input: '" << optarg << "', should be 'point', 'poly', 'line', 'dat' or 'comp'." << std::endl;
		returnValue = READ_ERROR_IFORMAT;
	}
	return returnValue;
}

enum error ofInit(enum out_format_t *outFormat, char *optarg) {
	enum error returnValue = SUCCESS;
	if (strcmp(optarg,"perm") == 0) *outFormat = OF_PERM;
	else if (strcmp(optarg,"poly") == 0) *outFormat = OF_POLY;
	else if (strcmp(optarg,"dat") == 0) *outFormat = OF_DAT;
	else if (strcmp(optarg,"line") == 0) *outFormat = OF_LINE;
	else if (strcmp(optarg,"pure") == 0) *outFormat = OF_PURE;
	else if (strcmp(optarg,"pp") == 0) *outFormat = OF_PURE_AND_PERM; // pp for pure and perm
	else {
		*outFormat = OF_UNDEFINED;
		std::cerr << "Error:  --outformat input incorrect.  Use -H for help. Input: '" << optarg << "', should be 'perm', 'dat', or 'poly'." << std::endl;
		returnValue = READ_ERROR_OFORMAT;
	}
	return returnValue;
}

enum error vFileInit(char *vFile, char *optarg) {
	enum error returnValue = SUCCESS;
	if(optarg[0] == 0) {
		std::cerr << "Error: no input file with polygonal data. Use -H for help." << std::endl;
		returnValue = NO_VERIFY_FILE;
	}
	else if (optarg[0] == '-') {
		std::cerr << "Error: another command set as input <string> for --verify. Use -H for help." << std::endl;
		returnValue = READ_ERROR_IFILE;
	}
	else {
		strcpy(vFile, optarg);
	}

	return returnValue;
}

enum error argInit(	int argc, char *argv[],
										char *inFile, char *outFile, enum alg_t *alg,
										enum in_format_t *inFormat, enum out_format_t *outFormat,
										bool& writeNew, bool& area,	bool& ch_area, bool& area_ratio, bool& perimeter, bool& norm_perimeter,
										unsigned int& randseed, bool& checkSimple, bool& generate_holes, unsigned int& holes,
										unsigned int& select_polygon, char *vFile, bool& run_tests, bool& help) {
	enum error returnValue = SUCCESS;
	int comm;

	static struct option long_options[] =
	{
		{"infile", required_argument, NULL, 'i'},
		{"outfile", required_argument, NULL, 'o'},
		{"alg", required_argument, NULL, 'a'},
		{"informat", required_argument, NULL, 'b'},
		{"outformat", required_argument, NULL, 'c'},
		{"hole", required_argument, NULL, 'h'},
		{"polygonfile", required_argument, NULL, 'p'},
		{"randseed", required_argument, NULL, 'r'},
		{"selectpolygon", required_argument, NULL, 's'},
		{"area", no_argument, NULL, 'd'},
		{"charea", no_argument, NULL, 'e'},
		{"charearatio", no_argument, NULL, 'f'},
		{"perimeter", no_argument, NULL, 'k'},
		{"normperimeter", no_argument, NULL, 'l'},
		{"test", no_argument, NULL, 't'},
		{"checksimple", no_argument, NULL, 'u'},
		{"writenew", no_argument, NULL, 'w'},
		{"help", no_argument, NULL, 'H'},
		{0, 0, 0, 0}
	};

	while(((comm = getopt_long (argc, argv, "i:o:a:b:c:h:p:r:s:defkltuwH", long_options, NULL)) != -1) && returnValue == SUCCESS) {
		switch(comm) {
			case 'i':
				returnValue = inFileInit(inFile, optarg);
//				std::cerr << "inFile returnValue: " << returnValue << std::endl;
				break;
			case 'o':
				returnValue = outFileInit(outFile, optarg);
//				std::cerr << "outFile returnValue: " << returnValue << std::endl;
				break;
			case 'a':
				returnValue = algInit(alg, optarg);
//				std::cerr << "algorithm returnValue: " << returnValue << std::endl;
				break;
			case 'b':
				returnValue = ifInit(inFormat, optarg);
//				std::cerr << "inFormat returnValue: " << returnValue << std::endl;
				break;
			case 'c':
				returnValue = ofInit(outFormat, optarg);
//				std::cerr << "outFormat returnValue: " << returnValue << std::endl;
				break;
			case 'h':
				generate_holes = true;
				holes = atoi(optarg);
				break;
			case 'd':
				area = true;
				break;
			case 'e':
				ch_area = true;
				break;
			case 'f':
				area_ratio = true;
				break;
			case 'k':
				perimeter = true;
				break;
			case 'l':
				norm_perimeter = true;
				break;
			case 'r':
				randseed = atoi(optarg);
				break;
			case 's':
				select_polygon = atoi(optarg);
				break;
			case 'u':
				checkSimple = true;
				break;
			case 't':
				run_tests = true;
				break;
			case 'p':
				returnValue = vFileInit(vFile, optarg);
				break;
			case 'w':
				writeNew = true;
				break;
			case 'H':
				help = true;
				std::cerr << "USAGE: ./spg -i in_file -b in_format -o out_file -c out_format -a algorithm" << std::endl;
				std::cerr << "Command line arguments:" << std::endl;
				std::cerr << " -H, --help" << std::endl;
				std::cerr << "             ignores any other argument and just prints this helpful information." << std::endl << std::endl;

				std::cerr << " -i, --infile <string> | (argument and string REQUIRED)" << std::endl;
				std::cerr << "              <string> is the filename of a file containing a set of points." << std::endl;
				std::cerr << "assumptions:  The file contains a point set where each point must be unique." << std::endl;
				std::cerr << "Exception: Some formats (line, points, dat) allow for multiple polygons, which are defined by repeating the first point of the polygon" << std::endl;
				std::cerr << "after the last point in the polygon before then starting a new polygon, this is allowed." << std::endl << std::endl;

				std::cerr << " -b, --informat <string> | (argument and string REQUIRED)" << std::endl;
				std::cerr << "                <string> can be:"  << std::endl;
				std::cerr << "                poly/pol : header with 'x_min x_max y_min y_max' in first line and number of points in 2nd line" << std::endl;
				std::cerr << "                           each subsequent line: 'x y'" << std::endl;
				std::cerr << "                comp     : each line: 'enumeration x y'" << std::endl;
				std::cerr << "                line     : header with number of points in the following polygon on a single line." << std::endl;
				std::cerr << "                           each subsequent line: ' x  y', supports multiple polygons." << std::endl;
				std::cerr << "                dat      : a gnuplot data file, header with # comments allowed." << std::endl;
				std::cerr << "                           each subsequent line: ' x  y', supports multiple polygons." << std::endl;
				std::cerr << "                point/pnt: no header, each line: 'x y', supports multiple polygons." << std::endl << std::endl;

				std::cerr << " -o. --outfile <string> | (argument optional*, if used then string required)" << std::endl;
				std::cerr << "               <string> is the filename of a file with the processed output of the program." << std::endl;
				std::cerr << " *if option skipped, will send polygons to stdout in the format given by '-c'." << std::endl << std::endl;

				std::cerr << " -c, --outformat <string> | (argument optional, if used then string required)" << std::endl;
				std::cerr << "                 <string> can be:" << std::endl;
				std::cerr << "                 perm : each line as an index/enumeration into the point set that was used." << std::endl;
				std::cerr << "                 poly : see description in 'informat'" << std::endl;
				std::cerr << "                 line : see description in 'informat'" << std::endl;
				std::cerr << "                 dat  : format that gnuplot understands and can plot." << std::endl;
				std::cerr << "*If this argument is missing, the program will do any/all calculations, but will not write to file or stdout." << std::endl << std::endl;

				std::cerr << " -a, --alg <string> | (argument optional, if used then string required)" << std::endl;
				std::cerr << "           <string> is the algorithm to be run:" << std::endl;
				std::cerr << "           2opt : calculates a simple random polygon based on Bentley-Ottman linesweep and the 2opt algorithm." << std::endl;
				std::cerr << "           5 : a faster calculation of a simple random polygon based on Bentley-Ottman linesweep and the 2opt algorithm." << std::endl;
				std::cerr << "           7 : the most randomised version of the linesweep algorithm." << std::endl;
				std::cerr << "           star : calculates a simple random polygon that is a star-shaped polygon." << std::endl;
				std::cerr << "           convert : converts a point set from one file format to another." << std::endl;
				std::cerr << "           verify : only checks whether a permutation of a point set is simple." << std::endl << std::endl;

				std::cerr << " -h, --hole <string> | (argument optional*, if used then string required)" << std::endl;
				std::cerr << "             <string> is the number of holes desired, '0' selects a random number of holes." << std::endl;
				std::cerr << " *By using '-h' you are free to either:" << std::endl;
				std::cerr << "  a) use the polygon as defined in the --infile (must be arranged as a simple polygon)." << std::endl;
				std::cerr << "  b) use --algorithm to select an algorithm to make a simple polygon on which to generate holes." << std::endl;
				std::cerr << "  c) use --polfile to give a file with a polygon of the point set defined in --infile (format: perm)" << std::endl << std::endl;

				std::cerr << " -p, --polfile <string> | (argument optional, if used then string required)" << std::endl;
				std::cerr << "               <string> is the filename of a file where each line is an index into the point set given by infile (perm)." << std::endl;
				std::cerr << " *By using '-p' you overwrite any/all polygons created by importing the point set from the infile." << std::endl << std::endl;

				std::cerr << " -r, --randseed <string> | (argument optional*, if used then string required)" << std::endl;
				std::cerr << "                <string> is an unsigned integer above 0." << std::endl;
				std::cerr << " *The seed is set at the beginning only and affects all options in this help that describes a random process." << std::endl << std::endl;

				std::cerr << "ON/OFF FLAGS:" << std::endl;
				std::cerr << " -w, --writenew" << std::endl;
				std::cerr << "    option to not overwrite the output file if it already exists," << std::endl;
				std::cerr << "    a new file is created with an increment number added to the end." << std::endl;
				std::cerr << " -d, --area" << std::endl;
				std::cerr << "    option to print the area of the polygon to cout." << std::endl;
				std::cerr << " -e, --charea" << std::endl;
				std::cerr << "    option to print the area of the convex hull to cout." << std::endl;
				std::cerr << " -f, --charearatio" << std::endl;
				std::cerr << "    option to print the ratio of (area of the polygon)/(area of convex hull) to cout." << std::endl;
				std::cerr << " -k, --perimeter" << std::endl;
				std::cerr << "    option to print the perimeter of the polygon to cout." << std::endl;
				std::cerr << " -l, --normperimeter" << std::endl;
				std::cerr << "    option to print the normalised perimeter of the polygon to cout. max([xmin,xmax],[ymin,ymax]) = [0,1]" << std::endl;
				std::cerr << " -t" << std::endl;
				std::cerr << "    ignores all other arguments and runs the test-bed." << std::endl;
				break;

			default:
				std::cerr << "Error:  input arguments not formatted correctly.  Use -H for help." << std::endl;
				returnValue = UNEXPECTED_ERROR;
		}
	}

	if (optind < argc) {
		printf ("non-option ARGV-elements: ");
		while (optind < argc)
			printf ("%s ", argv[optind++]);
		putchar ('\n');
	}

	return returnValue;
}
