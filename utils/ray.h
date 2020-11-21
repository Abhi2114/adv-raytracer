#pragma once

#include "vec3.h"

class ray {

private:

	vec3 origin, direction;

public:

	ray() {}

	ray(const vec3 &a, const vec3 &b) : origin(a), direction(b) {}

	vec3 getOrigin() const { return origin; }
	vec3 getDirection() const { return direction; }

	vec3 pointAtParameter(double t) const { return origin + t * direction; }
};
