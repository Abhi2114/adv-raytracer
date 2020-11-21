#pragma once

#include "..//utils/pdf.h"
#include "..//utils/onb.h"

// cosine BRDF for Lambertian surfaces
class cosinePDF : public PDF {

private:

	// orthonormal basis for random directions
	onb uvw;

	vec3 point, normal;

public:

	// construct an onb from w
	cosinePDF(const vec3& w) { uvw.buildFromNormal(w); }

	cosinePDF(const vec3& point, const vec3& normal) : point(point), normal(normal) {}

	virtual double value(const vec3& direction) const {

		double cosine = dot(unit_vector(direction), uvw.W());
		// double cosine = dot(unit_vector(direction), normal);

		if (cosine > 0)
			return cosine / PI;
		
		return 0;
	}

	virtual vec3 generate() const {

		return uvw.local(randomPointInHemisphere());
		// return normal + randomPointInSphere();
	}
};