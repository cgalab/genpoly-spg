#include "settings.h"

/*
	Polygon parameters
*/
int Settings::nrInnerPolygons = 0;
int Settings::outerSize = 100000;
std::vector<int> Settings::innerSizes;


/*
	Initial polygon
*/
int Settings::initialSize = 100;
double Settings::radius = 0.1;
double Settings::boxSize = 3.0;
int Settings::initialTranslationFactor = 1000;
int Settings::initialTranslationNumber = -1;


/*
	Translation parameters
*/
Arithmetics Settings::arithmetics = Arithmetics::DOUBLE;


/*
	Insertion parameters
*/
int Settings::insertionTries = 100;
double Settings::minLength = 0.0000001;


/*
	Utilities
*/
Timer* Settings::timer = NULL;


/*
	Random generator
*/
bool Settings::fixedSeed = false;
unsigned int Settings::seed = 0;
RandomGenerator* Settings::generator = NULL;


/*
	Feedback settings
*/
FeedbackMode Settings::feedback = FeedbackMode::EXECUTION;
bool Settings::simplicityCheck = false;

void Settings::initSettings(){
	// Generate and start Timer
	timer = new Timer();
	(*timer).start();

	// Compute the number of translations for the initial polygon
	if(initialSize < outerSize){
		initialTranslationNumber = initialTranslationFactor * initialSize;
	}else{
		initialTranslationNumber = initialTranslationFactor * outerSize;
		initialSize = outerSize;
	}

	// Initialize the RandomGenerator
	generator = new RandomGenerator(fixedSeed, seed);

	// Initialise the exact arithmetic
	if(arithmetics == Arithmetics::EXACT)
		exactinit();
}