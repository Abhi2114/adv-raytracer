#pragma once

#include "ray.h"
#include "Hitable.h"
#include "pdf.h"

// scatter record
struct ScatterRecord {

	ray scatteredRay;
	bool isDiffuse;
	vec3 attenuation;
	PDF* pdf;
};

// model the type of material
// helps us determine how a ray behaves when it hits a surface

class Material {

public:

	// attenuation - tells us how much the scattered ray needs to be attenuated
	virtual bool scatter(const ray& in, const HitRecord& record, vec3& attenuation, ray& scattered, double &pdf) const = 0;

	virtual bool scatter(const ray& in, const HitRecord& record, ScatterRecord &srecord) { return false; }

	virtual double scatteringPDF(const ray& in, const HitRecord& record, const ray& scattered) const {
		return 0.0;
	}

	// have some ambient light
	virtual vec3 emitted() const {

		return vec3(0.0, 0.0, 0.0);
	}
};