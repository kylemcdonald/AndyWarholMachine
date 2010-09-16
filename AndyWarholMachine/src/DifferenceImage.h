#pragma once

#include "FloatImage.h"

class DifferenceImage : public FloatImage {
protected:
	float avg, dev;
	
	void makeDifference(FloatImage& background, ofBaseImage& cur) {
		int n = size();		
		const float* backgroundPixels = background.getPixels();
		unsigned char* curPixels = cur.getPixels();
		for(int i = 0; i < n; i++)
			pixels[i] = backgroundPixels[i] - curPixels[i];
	}
	void makeStatistics() {
		int n = size();
		
		avg = 0;
		for(int i = 0; i < n; i++)
			avg += pixels[i];
		avg /= n;
		
		dev = 0;
		for(int i = 0; i < n; i++) {
			float diff = avg - pixels[i];
			dev += diff * diff;
		}
		dev = sqrtf(dev / n);
	}
public:
	void makeAbsoluteDifference(FloatImage& background, ofBaseImage& cur) {
		makeDifference(background, cur);
		int n = size();
		for(int i = 0; i < n; i++)
			if(pixels[i] < 0)
				pixels[i] = -pixels[i];
		makeStatistics();
	}
	float getAverage() {
		return avg;
	}
	float getStandardDeviation() {
		return dev;
	}
};