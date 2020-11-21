#pragma once

#include "..//utils/Material.h"

class Diffuse : public Material {

private:

	vec3 color;  // color of the light

public:

	Diffuse(const vec3& color) : color(color) {}

	// no scattering if we hit a light source
	virtual bool scatter(const ray& in, const HitRecord& record, vec3& attenuation, ray& scattered, double& pdf) const {
		return false;
	}

	virtual vec3 emitted() const {
		return color;
	}
};