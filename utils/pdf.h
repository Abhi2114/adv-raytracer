#pragma once

#include "vec3.h"

// generic pdf

class PDF {

public:

	virtual double value(const vec3& direction) const = 0;  // pdf value
	virtual vec3 generate() const = 0;  // pdf direction
};