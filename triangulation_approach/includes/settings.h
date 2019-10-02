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
		Initial polygon
	*/

	// Size of the initial polygon
	static int initialSize;

	// Radius of the initial polygon
	static double radius;

	// Size of the bounding box
	static double boxSize;

	// Average number of translations per vertex of the initial polygon
	static int initialTranslationFactor;

	// Number of translations to perform on the initial polygon
	static int initialTranslationNumber;


	/*
		Translation parameters
	*/

	// Minimal time distance between events in the event queue
	static constexpr double epsEventTime = 0.00001; //10^-5

	// At intersection check all determinants' absolute values less then epsInt
	// are interpreted as zero
	static constexpr double epsInt = 0.000000000001; // 10⁻¹²

	// Minimal allowed determinant for triangles attached to edges for insertion
	// To small triangles on the edge of insertion can lead to the vertex moving
	// out of its surrounding polygon (just with double arithmetics)
	static constexpr double minDetInsertion = 0.000000000001; // 10⁻¹²

	static Arithmetics arithmetics;

	/*
		Polygon growth
	*/

	// Target size of the polygon
	static int targetSize;

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

	// Function for initialising the settings
	static void initSettings();

	/*
		Check
	*/

	// Enable simplicity check
	static bool simplicityCheck;
};

#endif