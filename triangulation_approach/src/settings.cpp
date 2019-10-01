#include "settings.h"

int Settings::initialSize = 100;
double Settings::radius = 0.1;
double Settings::boxSize = 3.0;
int Settings::initialTranslationFactor = 1000;
int Settings::initialTranslationNumber = -1;
Arithmetics Settings::arithmetics = Arithmetics::DOUBLE;
int Settings::targetSize = 1000000;
int Settings::insertionTries = 100;
double Settings::minLength = 0.0000001;
Timer* Settings::timer = NULL;
bool Settings::fixedSeed = false;
unsigned int Settings::seed = 2983747647;
RandomGenerator* Settings::generator = NULL;
FeedbackMode Settings::feedback = FeedbackMode::EXECUTION;
bool Settings::simplicityCheck = false;

void Settings::initSettings(){
	// Generate and start Timer
	timer = new Timer();
	(*timer).start();

	// Compute the number of translations for the initial polygon
	if(initialSize < targetSize){
		initialTranslationNumber = initialTranslationFactor * initialSize;
	}else{
		initialTranslationNumber = initialTranslationFactor * targetSize;
		initialSize = targetSize;
	}

	// Initialize the RandomGenerator
	generator = new RandomGenerator(fixedSeed, seed);

	// Initialize the exact arithmetic
	if(arithmetics == Arithmetics::EXACT)
		exactinit();
}