#pragma once

#include "..//utils/Material.h"
#include "..//utils/funcs.h"
#include "..//utils/onb.h"
#include "..//pdfs/cosinePDF.h"

// diffuse/matte surfaces!
class Lambertian : public Material {

private:

	vec3 albedo;  // gives the color

public:

	Lambertian(const vec3& albedo) : albedo(albedo) {}

	// standard BRDF for lambertian surfaces, since every direction is equally likely
	virtual double scatteringPDF(const ray& in, const HitRecord& record, const ray& scattered) const {

		// lambert's cosine law
		double cosine = dot(record.normal, unit_vector(scattered.getDirection()));

		if (cosine < 0) cosine = 0;

		return cosine / PI;
	}

	virtual bool scatter(const ray& in, const HitRecord& record, vec3& attenuation, ray& scattered, double& pdf) const {

		/*
		/// OLD

		// get a random direction bouncing off the surface, ideal for modelling diffuse surfaces
		vec3 target = record.point + record.normal + randomPointInSphere();
		scattered = ray(record.point, target - record.point);

		if (albedo.r() == 0 && albedo.g() == 0 && albedo.b() == 0) {
			// checker pattern
			int m = int(std::floor(record.point.x() / 70.0) + std::floor(record.point.z() / 70.0));

			if (m % 2 == 0) attenuation = vec3(0.7, 0.6, 0.1);
			else attenuation = vec3(0.1, 0.2, 0.8);
		}
		else
			attenuation = albedo;

		// set pdf for outgoing scattered ray
		pdf = dot(record.normal, unit_vector(scattered.getDirection())) / PI;
		*/

		onb uvw;

		uvw.buildFromNormal(record.normal);
		vec3 direction = uvw.local(randomCosineDirection());
		scattered = ray(record.point, unit_vector(direction));

		if (albedo.r() == 0 && albedo.g() == 0 && albedo.b() == 0) {
			// checker pattern
			int m = int(std::floor(record.point.x() / 70.0) + std::floor(record.point.z() / 70.0));

			if (m % 2 == 0) attenuation = vec3(0.7, 0.6, 0.1);
			else attenuation = vec3(0.1, 0.2, 0.8);
		}
		else
			attenuation = albedo;

		pdf = dot(record.normal, scattered.getDirection()) / PI;

		return true;
	}

	virtual bool scatter(const ray& in, const HitRecord& record, ScatterRecord& srecord) {

		// fill in scatter record
		srecord.isDiffuse = true;

		// set the attenuation
		if (albedo.r() == 0 && albedo.g() == 0 && albedo.b() == 0) {
			// checker pattern
			int m = int(std::floor(record.point.x() / 70.0) + std::floor(record.point.z() / 70.0));

			if (m % 2 == 0) srecord.attenuation = vec3(0.7, 0.6, 0.1);
			else srecord.attenuation = vec3(0.1, 0.2, 0.8);
		}
		else
			srecord.attenuation = albedo;

		// set the pdf as well
		srecord.pdf = new cosinePDF(record.normal);

		return true;
	}
};
