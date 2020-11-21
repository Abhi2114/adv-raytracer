#pragma once

#include "ray.h"
#include "AABB.h"

class Material;

// info we need if a ray hits an object
struct HitRecord {

	double distance = 0;
	vec3 point;
	vec3 normal;
	Material* material = nullptr;
};

// abstract class every "hitable" object needs to extend
class Hitable {

public:

	// validate hits between (tMin, tMax)
	virtual bool isHit(const ray& r, double tMin, double tMax, HitRecord& record) const = 0;
	// make a bounding box with this hitable
	virtual bool makeBB(AABB &box) const = 0;

	virtual double pdfValue(const vec3& o, const vec3& v) const { return 0.0; }
	virtual vec3 random(const vec3& o) const { return vec3(1, 0, 0); }

	virtual Hitable* getHitable (size_t index) { return nullptr; }
};

// flip normals
class FlipNormals : public Hitable {

private:

	Hitable* hitable;

public:

	FlipNormals(Hitable *hitable) : hitable(hitable) {}

	virtual bool isHit(const ray& r, double tMin, double tMax, HitRecord& record) const {

		// just a small wrapper to modify the returned normal from the hit
		if (hitable->isHit(r, tMin, tMax, record)) {
			// flip the normal
			record.normal = -record.normal;
			return true;
		}

		return false;
	}

	virtual bool makeBB(AABB& box) const {
		
		return hitable->makeBB(box);
	}
};