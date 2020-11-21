#pragma once


#include "..//utils/Material.h"

#include "..//utils/funcs.h"

// metallic surfaces!
class Metal : public Material {

private:

	vec3 albedo;
	double fuzziness;  // how shiny is the surface?

public:

	Metal(const vec3& albedo, double fuzziness = 0) : albedo(albedo), fuzziness(fuzziness) {}

	virtual bool scatter(const ray& in, const HitRecord& record, vec3& attenuation, ray& scattered, double& pdf) const {

		// reflections for metals!
		vec3 reflected = reflect(unit_vector(in.getDirection()), record.normal);
		// the new scattered ray originates from the hitpoint and goes in the direction of reflection
		scattered = ray(record.point, reflected + fuzziness * randomPointInSphere());
		attenuation = albedo;

		return dot(reflected, record.normal);
	}

	virtual bool scatter(const ray& in, const HitRecord& record, ScatterRecord& srecord) {

		// reflections for metals!
		vec3 reflected = reflect(unit_vector(in.getDirection()), record.normal);

		// the new scattered ray originates from the hitpoint and goes in the direction of reflection
		srecord.scatteredRay = ray(record.point, reflected + fuzziness * randomPointInSphere());
		srecord.attenuation = albedo;
		srecord.pdf = nullptr;
		srecord.isDiffuse = false;

		return dot(reflected, record.normal);
	}
};