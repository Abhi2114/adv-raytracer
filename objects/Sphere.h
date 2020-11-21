#pragma once

#include "..//utils/Hitable.h"
#include "..//utils/Material.h"
#include "..//utils/funcs.h"
#include "..//utils/onb.h"
#include "..//utils/AABB.h"

class Sphere : public Hitable {

private:

	vec3 center;
	double radius;
	Material* material;

	bool testRange(double distance, const ray& r, double tMin, double tMax, HitRecord& record) const {

		if (tMin < distance && distance < tMax) {
			// record the hit info
			record.distance = distance;
			// scale the ray by the distance to get the intersection point
			record.point = r.pointAtParameter(distance);
			// computing the normal is easy
			record.normal = (record.point - center) / radius;
			// set the material as well
			record.material = material;

			return true;
		}

		return false;
	}

public:

	Sphere() : center(vec3(-1, -1, -1)), radius(0), material(nullptr) {}

	Sphere(const vec3 &center, double radius, Material *material = nullptr) : 
		center(center), 
		radius(radius),
		material(material) {}

	virtual bool isHit(const ray& r, double tMin, double tMax, HitRecord& record) const {

		vec3 oc = r.getOrigin() - center;
		// get the quadratic co-efficients
		double a = dot(r.getDirection(), r.getDirection());
		double b = dot(oc, r.getDirection());
		double c = dot(oc, oc) - radius * radius;

		// solve!
		double discriminant = b * b - a * c;

		if (discriminant > 0) {
			// we have a hit! validate hit with the given range
			double distance = (-b - sqrt(discriminant)) / a;

			if (testRange(distance, r, tMin, tMax, record))
				return true;

			// try again
			distance = (-b + sqrt(discriminant)) / a;

			return testRange(distance, r, tMin, tMax, record);
		}

		// no hit
		return false;
	}

	// bounding box around a sphere is easy
	virtual bool makeBB(AABB& box) const {

		box = AABB(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
		return true;
	}

	virtual double pdfValue(const vec3& o, const vec3& v) const { 
		
		// blast a ray toward the sphere
		HitRecord record;

		if (isHit(ray(o, v), 0.001, MAXDOUBLE, record)) {

			// pdf is just the reciprocal of the solid angle of the sphere
			double cosThetaMax = sqrt(1 - radius * radius / (center - o).length_squared());
			double solidAngle = 2 * PI * (1 - cosThetaMax);
			return 1.0 / solidAngle;
		}

		return 0.0;
	}

	virtual vec3 random(const vec3& o) const {

		vec3 direction = center - o;

		double ds = direction.length_squared();

		onb uvw;
		uvw.buildFromNormal(direction);

		// convert random direction to sphere to local coordinates
		return uvw.local(randomToSphere(radius, ds));
	}
};
