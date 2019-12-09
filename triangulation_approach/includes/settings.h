#include <vector>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>

#include "timer.h"
#include "randomGenerator.h"
#include "predicates.h"

#ifndef __SETTINGS_H_
#define __SETTINGS_H_

enum class FeedbackMode {LACONIC, EXECUTION, VERBOSE};
enum class Arithmetics {EXACT, DOUBLE};

class Settings{

public:
	
	/*
		GLOBAL VARIABLES
	*/


	/*
		Polygon parameters
	*/

	// The number of the inner polygons
	static unsigned int nrInnerPolygons;

	// Target size of the outer polygon
	static int outerSize;

	// Target sizes of the inner polygons
	static std::vector<int> innerSizes;


	/*
		Initial polygon
	*/

	// Size of the initial polygon
	static int initialSize;

	// Radius of the initial outer polygon
	static double radiusPolygon;

	// Radius of the first initial hole
	static double radiusHole;

	// Size of the bounding box
	static double boxSize;

	// Average number of translations per vertex of the initial polygon
	static int initialTranslationFactor;

	// Number of translations to perform on the initial polygon
	static int initialTranslationNumber;


	/*
		Translation parameters
	*/

	// Type of used arithmetics
	static Arithmetics arithmetics;

	// Minimal time distance between events in the event queue
	static constexpr double epsEventTime = 0.00001; //10⁻⁵

	// At intersection check all determinants' absolute values less then epsInt
	// are interpreted as zero
	static constexpr double epsInt = 0.000000000001; // 10⁻¹²

	// Minimal allowed determinant for triangles attached to edges for insertion
	// To small triangles on the edge of insertion can lead to the vertex moving
	// out of its surrounding polygon (just with double arithmetics)
	static constexpr double minDetInsertion = 0.000000000001; // 10⁻¹²

	// Enable local correctness checking after aborted translations
	static bool localChecking;

	// Enable global correctness checking between stages
	static bool globalChecking;


	/*
		Insertion parameters
	*/

	// Number of tries to move an inserted vertex
	static int insertionTries;

	// Minimal allowed edge length for insertions
	static double minLength;


	/*
		Utilities
	*/

	// Global Timer
	static Timer *timer;


	/*
		Random Generator
	*/

	// Enable a fixed seed for the RandomGenerator
	static bool fixedSeed;

	// The fixed seed for the RandomGenerator (if enabled)
	static unsigned int seed;

	// Global RandomGenerator
	static RandomGenerator *generator;


	/*
		Feedback settings
	*/

	// Feedback mode
	static FeedbackMode feedback;

	// Flag whether execution info should be printed
	static bool executionInfo;

	// Flag whether info an numerical correction should be printed
	static bool correctionInfo;

	// Flag to mute all output
	static bool mute;

	// Filename for polygon output file
	static char *polygonFile;

	// A flag whether the triangulation is required
	static bool triangulationOutputRequired;

	// Filename for triangulation file
	static char *triangulationFile;

	// Filename for statistics file
	static char *statisticsFile;





	// Function for reading the configuration file
	static void readConfigFile(char *filename);

	// Function for printing the settings
	static void printSettings();

	// cast string to char*
	static char* stringToChar(std::string str);

	// Reads booleans
	static bool readBoolean(bool &found);

	// Read arithmetic type
	static Arithmetics readArithmeticType(bool &found);

	// Checks whether all necessary settings are given
	static void checkAndApplySettings();

	// Get hole sizes
	static void readHoleSizes(bool &found);

	//Print a dummy configuration file
	static void printDummyFile();

	/*
		Check
	*/

	// Enable simplicity check
	static bool simplicityCheck;
};

#endif