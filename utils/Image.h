#pragma once

#include "vec3.h"

// imp: stored in row-major order
class Image {

private:

	// some meta data
	int width, height;

	// the actual content
	vec3* buffer;

public:

	Image(int width, int height) : width(width), height(height) {
		buffer = new vec3[width * height];
	}

	// get pixel at given position
	vec3 get(int i, int j) const {  // i -> width, j -> height

		if (-1 < i && i < width &&
			-1 < j && j < height)
			return buffer[j * width + i];

		return vec3(-1, -1, -1);
	}

	void set(int i, int j, const vec3& color) {

		if (-1 < i && i < width &&
			-1 < j && j < height)
			buffer[j * width + i] = color;
	}

	// getter info
	int getWidth() const { return width; }

	int getHeight() const { return height; }

	~Image() { delete[] buffer; }

	// write image in buffer to disk as a PPM
	void write();
};
