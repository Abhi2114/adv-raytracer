#include "Image.h"

#include <fstream>

// write PPM
void Image::write() {

	std::ofstream image;

	image.open("output.ppm");

	// meta-data
	image << "P3\n";
	image << width << " " << height << "\n";
	image << "255\n";

	// actual content
	for (int i = 0; i < width * height; ++i) {
		vec3 color = buffer[i];
		// write indi color
		image << 255.0 * color.r() << " " << 255.0 * color.g() << " " << 255.0 * color.b() << "\n";
	}

	image.close();
}
