#include "DifferenceImage.h"

void DifferenceImage::makeDifference(FloatImage& background, ofBaseImage& cur) {
	int n = size();		
	const float* backgroundPixels = background.getPixels();
	unsigned char* curPixels = cur.getPixels();
	for(int i = 0; i < n; i++)
		pixels[i] = backgroundPixels[i] - curPixels[i];
}

void DifferenceImage::makeStatistics() {
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

void DifferenceImage::makeAbsoluteDifference(FloatImage& background, ofBaseImage& cur) {
	makeDifference(background, cur);
	int n = size();
	for(int i = 0; i < n; i++)
		if(pixels[i] < 0)
			pixels[i] = -pixels[i];
	makeStatistics();
}

float DifferenceImage::getAverage() {
	return avg;
}

float DifferenceImage::getStandardDeviation() {
	return dev;
}