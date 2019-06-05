#include "timer.h"

#ifndef __SETTINGS_H_
#define __SETTINGS_H_

enum class FeedbackMode {LACONIC, EXECUTION, VERBOSE};

class Settings{

private:
	FeedbackMode feedback;

	// initial polygon
	int initialSize;
	double radius;
	double boxSize;
	
	// translation of the initial polygon
	int initialTranslationNumber;
	
	// polygon growth
	int targetSize;
	int insertionTries;

	Timer t;

public:
	Settings();

	// Getter
	FeedbackMode getFBMode();
	int getInitialSize();
	double getRadius();
	double getBoxSize();
	int getInitialTN();
	int getTargetSize();
	int getInsertionTries();

	double elapsedTime();

	// Setter
	void setFBMode(FeedbackMode fb);
	void setInitialSize(int n);
	void setRadius(double r);
	void setBoxSize(double s);
	void setInitialTN(int factor);
	void setTargetSize(int n);
	void setInsertionTries(int n);




};

#endif

void readSettings(Settings &settings);