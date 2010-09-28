#pragma once

#include "FloatImage.h"

class DifferenceImage : public FloatImage {
protected:
	float avg, dev;
	
	void makeDifference(FloatImage& background, ofBaseImage& cur);
	void makeStatistics();
public:
	void makeAbsoluteDifference(FloatImage& background, ofBaseImage& cur);
	float getAverage();
	float getStandardDeviation();
};