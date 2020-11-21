#pragma once

// creds: Peter Shirley, Ray Tracing in a Weekend

#include "..//utils/Material.h"

#include "..//utils/funcs.h"


// an attempt at modelling a glass ball
class Dielectric : public Material {

private:

	double rIndex;  // refracted index of the dielectric material

public:

	Dielectric() : rIndex(1) {}  // default is vaccuum

	Dielectric(double rIndex) : rIndex(rIndex) {}

	virtual bool scatter(const ray& in, const HitRecord& record, vec3& attenuation, ray& scattered, double& pdf) const {

		// outward normal to the surface
		vec3 outwardNormal;
		// refractive index
		double index;

		// the above 2 pieces of information will change depending upon if we are inside the surface or outside

		// fix the attenuation
		attenuation = vec3(1, 1, 1);

		vec3 reflected = reflect(in.getDirection(), record.normal);

		vec3 refracted;
		double reflectionProb, cosine;

		if (dot(in.getDirection(), record.normal) > 0) {
			// inside to outside
			outwardNormal = -record.normal;
			index = rIndex;
			cosine = index * dot(in.getDirection(), record.normal) / in.getDirection().length();
		}
		else {
			// outside to inside
			outwardNormal = record.normal;
			index = 1.0 / rIndex;
			cosine = -dot(in.getDirection(), record.normal) / in.getDirection().length();
		}

		// do the refraction calc now
		if (refract(in.getDirection(), outwardNormal, index, refracted))
			reflectionProb = schlick(cosine, rIndex);
		else // refraction not possible, return reflected ray instead
			reflectionProb = 1.0;
		
		if (genRandom(0.0, 1.0) < reflectionProb)  // reflect
			scattered = ray(record.point, reflected);
		else  // refract
			scattered = ray(record.point, refracted);

		return true;
	}

	virtual bool scatter(const ray& in, const HitRecord& record, ScatterRecord& srecord) {

		// outward normal to the surface
		vec3 outwardNormal;
		// refractive index
		double index;

		// the above 2 pieces of information will change depending upon if we are inside the surface or outside

		// fix the attenuation
		srecord.attenuation = vec3(1, 1, 1);

		vec3 reflected = reflect(in.getDirection(), record.normal);

		vec3 refracted;
		double reflectionProb, cosine;

		if (dot(in.getDirection(), record.normal) > 0) {
			// inside to outside
			outwardNormal = -record.normal;
			index = rIndex;
			cosine = index * dot(in.getDirection(), record.normal) / in.getDirection().length();
		}
		else {
			// outside to inside
			outwardNormal = record.normal;
			index = 1.0 / rIndex;
			cosine = -dot(in.getDirection(), record.normal) / in.getDirection().length();
		}

		// do the refraction calc now
		if (refract(in.getDirection(), outwardNormal, index, refracted))
			reflectionProb = schlick(cosine, rIndex);
		else // refraction not possible, return reflected ray instead
			reflectionProb = 1.0;

		if (genRandom(0.0, 1.0) < reflectionProb)  // reflect
			srecord.scatteredRay = ray(record.point, reflected);
		else  // refract
			srecord.scatteredRay = ray(record.point, refracted);

		srecord.isDiffuse = false;
		srecord.pdf = nullptr;

		return true;
	}
};