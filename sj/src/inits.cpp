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
	Functions that deal with possible errors in the command line argument parsing
*/

enum error inFileInit(char *inFile, char *optarg) {
	enum error returnValue = SUCCESS;
	if(optarg[0] == 0) {
		std::cerr << "Error: no input entered for inFile. Use -? for help." << std::endl;
		returnValue = NO_IN_FILE;
	}
	else if (optarg[0] == '-') {
		std::cerr << "Error: another command set as input <string> for --infile. Use -? for help." << std::endl;
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
		std::cerr << "Error: no input entered for outFile. Use -? for help." << std::endl;
		returnValue = NO_OUT_FILE;
	}
	else */
	if (optarg[0] == '-') {
		std::cerr << "Error: another command set as input <string> for --outfile. Use -? for help." << std::endl;
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
	else if(strcmp(optarg,"curve") == 0) {
		*alg = A_CURVE;
	}
	else if(strcmp(optarg,"hole") == 0) {
		*alg = A_HOLE;
	}
	else {
		*alg = A_UNDEFINED;
		std::cerr << "Error:  --alg input incorrect.  Use -? for help. Input: '" << optarg << "', should be '2opt'." << std::endl;
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
		std::cerr << "Error:  --informat input incorrect.  Use -? for help. Input: '" << optarg << "', should be 'points', 'poly', or 'comp'." << std::endl;
		returnValue = READ_ERROR_IFORMAT;
	}
	return returnValue;
}

enum error ofInit(enum out_format_t *outFormat, char *optarg) {
	enum error returnValue = SUCCESS;
	if (strcmp(optarg,"perm") == 0) *outFormat = OF_PERM;
	else if (strcmp(optarg,"poly") == 0) *outFormat = OF_POLY;
	else if (strcmp(optarg,"dat") == 0) *outFormat = OF_DAT;
	else if (strcmp(optarg,"pure") == 0) *outFormat = OF_PURE;
	else {
		*outFormat = OF_UNDEFINED;
		std::cerr << "Error:  --outformat input incorrect.  Use -? for help. Input: '" << optarg << "', should be 'perm', 'dat', or 'poly'." << std::endl;
		returnValue = READ_ERROR_OFORMAT;
	}
	return returnValue;
}

enum error argInit(	int argc, char *argv[],
										char *inFile, char *outFile, enum alg_t *alg,
										enum in_format_t *inFormat, enum out_format_t *outFormat,
										bool& writeNew, bool& area,	double& areaMin, double& areaMax,
										unsigned int& randseed, bool& checkSimple, unsigned int& holes) {
	enum error returnValue = SUCCESS;
	int comm;

	static struct option long_options[] =
	{
		{"infile", required_argument, NULL, 'i'},
		{"outfile", required_argument, NULL, 'o'},
		{"alg", required_argument, NULL, 'a'},
		{"informat", required_argument, NULL, 'b'},
		{"outformat", required_argument, NULL, 'c'},
		{"areamin", optional_argument, NULL, 'n'},
		{"areamax", optional_argument, NULL, 'x'},
		{"randseed", required_argument, NULL, 'r'},
		{"writenew", no_argument, NULL, 'w'},
		{"simplecalc", no_argument, NULL, 's'},
		{"holes", no_argument, NULL, 'h'},
		{"help", no_argument, NULL, '?'},
		{0, 0, 0, 0}
	};

	while( (comm = getopt_long (argc, argv, "i:o:a:b:c:h:r:w?stn::x::", long_options, NULL)) != -1 ) {
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
			case 'h':
				holes = atoi(optarg);
				break;
			case 'n':
				area = true;
				if (optarg) areaMin = atof(optarg);
				else areaMin = 0;
				break;
			case 'r':
				randseed = atoi(optarg);
				break;
			case 's':
				checkSimple = true;
				break;
			case 't':
				returnValue = RUN_TESTS;
				break;
			case 'w':
				writeNew = true;
				break;
			case 'x':
				area = true;
				if (optarg) areaMax = atof(optarg);
				else areaMax = INFINITY;
				break;
			case '?':
				returnValue = NO_ARGUMENTS;
				std::cerr << "Command line arguments:" << std::endl;
				std::cerr << " -?, --help" << std::endl;
				std::cerr << "             ignores any other argument and just prints this helpful information." << std::endl << std::endl;
				std::cerr << " -i, --infile <string>" << std::endl;
				std::cerr << "              <string> is the filename of a file containing a set of points." << std::endl << std::endl;
				std::cerr << " -o. --outfile <string>" << std::endl;
				std::cerr << "               <string> is the filename of a file with the processed output of the program." << std::endl << std::endl;
				std::cerr << " -a, --alg <arg>" << std::endl;
				std::cerr << "           <arg> can be '2opt' or 'hole' (without the '')." << std::endl << std::endl;
				std::cerr << " -b, --informat <arg>" << std::endl;
				std::cerr << "                <arg> can be:"  << std::endl;
				std::cerr << "                     'points' : no header, each line: 'x y'" << std::endl;
				std::cerr << "                     'poly'   : header with 'x_min x_max y_min y_max' in first line and number of points in 2nd" << std::endl;
				std::cerr << "                                each subsequent line: 'x y'" << std::endl;
				std::cerr << "                     'comp'   : header with comments, each subsequent line: 'enumeration x y'" << std::endl << std::endl;
				std::cerr << " -c, --outformat <arg>" << std::endl;
				std::cerr << "                 <arg> can be:" << std::endl;
				std::cerr << "                     'perm'   : each line as an index into the point set that was used." << std::endl;
				std::cerr << "                     'poly'   : same as above for 'informat'" << std::endl;
				std::cerr << "                     'dat'    : format that gnuplot understands and can plot." << std::endl << std::endl;
				std::cerr << " -h, --holes <arg>" << std::endl;
				std::cerr << "             <arg> is the number of holes desired." << std::endl << std::endl;
				std::cerr << " -w, --writenew" << std::endl;
				std::cerr << "             option to not overwrite the output file if it already exists," << std::endl;
				std::cerr << "             a new file is created with an increment number added to the end." << std::endl << std::endl;
				std::cerr << " -n, --areamin<arg>" << std::endl;
				std::cerr << "              no space allowed between areamin and <arg> or n and <arg> as it's an optional argument" << std::endl;
				std::cerr << "              the polygon is recalculated until returned area is above given minimum area" << std::endl;
				std::cerr << "              if no argument is given, will simply calculate the area and output to cout" << std::endl << std::endl;
				std::cerr << " -x, --areamax<arg>" << std::endl;
				std::cerr << "              no space allowed between areamax and <arg> or x and <arg> as it's an optional argument" << std::endl;
				std::cerr << "              the polygon is recalculated until returned area is below given maximum area" << std::endl;
				std::cerr << "              if no argument is given, will simply calculate the area and output to cout" << std::endl << std::endl;
				std::cerr << " -r, --randseed <arg>" << std::endl;
				std::cerr << "                <arg> is an unsigned integer above 0." << std::endl << std::endl;
				std::cerr << " -s, --simplecalc" << std::endl;
				std::cerr << "              uses a really slow check to verify simplicity and count intersections (if any)" << std::endl;
				std::cerr << " -t" << std::endl;
				std::cerr << "   ignores all other arguments and runs the test-bed." << std::endl;
				break;

			default:
				std::cerr << "Error:  input arguments not formatted correctly.  Use -? for help." << std::endl;
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
