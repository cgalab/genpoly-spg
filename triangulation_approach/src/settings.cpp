#include "settings.h"

// Constructors
// a boxsize of 1000 has precision around 10⁻¹³
Settings::Settings() :
feedback(FeedbackMode::EXECUTION), initialSize(100), radius(0.1), boxSize(3), 
insertionTries(100), t(Timer()) {
	
	t.start();
}

// Getter
FeedbackMode Settings::getFBMode(){
	return feedback;
}

int Settings::getInitialSize(){
	return initialSize;
}

double Settings::getRadius(){
	return radius;
}

double Settings::getBoxSize(){
	return boxSize;
}

int Settings::getInitialTN(){
	return initialTranslationNumber;
}

int Settings::getTargetSize(){
	return targetSize;
}

int Settings::getInsertionTries(){
	return insertionTries;
}

double Settings::elapsedTime(){
	return t.elapsedTime();
}

// Setter
void Settings::setFBMode(FeedbackMode fb){
	feedback = fb;
}

void Settings::setInitialSize(int n){
	initialSize = n;
}

void Settings::setRadius(double r){
	radius = r;
}

void Settings::setBoxSize(double s){
	boxSize = s;
}

void Settings::setInitialTN(int n){
	initialTranslationNumber = n;
}

void Settings::setTargetSize(int n){
	targetSize = n;	
}

void Settings::setInsertionTries(int n){
	insertionTries = n;
}


// static stuff
void readSettings(Settings &settings){
	int n = 1000000;
	int initialN = 100;
	int initTrans;
	int factor = 1000;

	settings.setInitialSize(initialN);
	settings.setTargetSize(n);
	settings.setFBMode(FeedbackMode::EXECUTION);

	if(n > initialN)
		initTrans = factor * initialN;
	else
		initTrans = factor * n;

	settings.setInitialTN(initTrans);
}