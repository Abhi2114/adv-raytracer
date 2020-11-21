#pragma once

#include "..//utils/pdf.h"
#include "..//utils/Hitable.h"

// sample directions toward a hitable object (a light source, for the most part, but could be any object)
class HitablePDF : public PDF {

private:

	vec3 origin;  // origin of the sampled ray
	Hitable* hitable;  // this will be the light source for the most part

public:

	HitablePDF(const vec3& origin, Hitable* hitable) : origin(origin), hitable(hitable) {}

	virtual double value(const vec3& direction) const {
		return hitable->pdfValue(origin, direction);
	}

	virtual vec3 generate() const {
		return hitable->random(origin);
	}
};