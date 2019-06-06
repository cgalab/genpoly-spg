#include "settings.h"

// Constructors
Settings::Settings() :
feedback(FeedbackMode::EXECUTION), initialSize(100), radius(100), boxSize(500), 
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
	int n = 10000;
	int initialN = 100;
	int initTrans;
	int factor = 1000;

	settings.setInitialSize(initialN);
	settings.setTargetSize(n);
	settings.setFBMode(FeedbackMode::LACONIC);

	if(n > initialN)
		initTrans = factor * initialN;
	else
		initTrans = factor * n;

	settings.setInitialTN(initTrans);
}