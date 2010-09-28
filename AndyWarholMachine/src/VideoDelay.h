#pragma once

#include "VideoBuffer.h"

class VideoDelay : public VideoBuffer {
protected:
	int position;
public:
	VideoDelay() :
	position(0) {
	}
	void add(ofBaseImage& img) {
		set(position, img);
		position++;
		if(position == buffer.size())
			position = 0;
	}
	ofImage& get(int i) {
		return VideoBuffer::get((position + buffer.size() - i - 1) % buffer.size());
	}
};