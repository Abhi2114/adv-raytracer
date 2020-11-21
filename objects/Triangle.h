#pragma once

#include <algorithm>
#include "..//utils/Hitable.h"
#include "..//utils/Material.h"
#include "..//utils/funcs.h"
#include "..//utils/onb.h"

// another super important primitive
class Triangle : public Hitable {

private:

	vec3 a, b, c;     // counter-clockwise order
	vec3 na, nb, nc;  // corresponding vertex normals
	Material* material;

public:

	Triangle(const vec3& a, const vec3& b, const vec3& c, Material *material) :
		a(a), b(b), c(c), material(material) {}

	Triangle(const vec3& a, const vec3& b, const vec3& c, 
			 const vec3& na, const vec3& nb, const vec3& nc,
			 Material* material) :
		a(a), b(b), c(c), material(material) {
	
		// normalize the normals
		this->na = unit_vector(na);
		this->nb = unit_vector(nb);
		this->nc = unit_vector(nc);
	}

	virtual bool isHit(const ray& r, double tMin, double tMax, HitRecord& record) const {

		vec3 rayOrigin = r.getOrigin();
		vec3 rayDirection = r.getDirection();

		vec3 e1 = b - a;
		vec3 e2 = c - a;

		// get the barycentric co-ordinates of the intersection point
		double u = dot((rayOrigin - a), cross(rayDirection, e2)) / dot(e1, cross(rayDirection, e2));
		double v = dot(rayDirection, (cross(rayOrigin - a, e1)) / dot(e1, cross(rayDirection, e2)));
		double w = 1 - u - v;

		if ((u < 0) || (u > 1) || (v < 0) || (u + v > 1))
			return false;

		// compute the ray distance
		double d = dot(e2, cross(rayOrigin - a, e1)) / dot(e1, cross(rayDirection, e2));

		if (d < tMin || d > tMax) 
			return false;

		// pack all the useful info up
		record.distance = d;
		record.point = r.pointAtParameter(d);
		// use the face normal for now, will use barycentric interpolation later
		record.normal = unit_vector(w * na + u * nb + v * nc);
		
		// record.normal = unit_vector(cross(e1, e2));

		record.material = material;

		return true;
	}

	virtual bool makeBB(AABB& box) const {

		vec3 min(std::min(std::min(a.x(), b.x()), c.x()), 
				 std::min(std::min(a.y(), b.y()), c.y()),
				 std::min(std::min(a.z(), b.z()), c.z()));

		vec3 max(std::max(std::max(a.x(), b.x()), c.x()),
				 std::max(std::max(a.y(), b.y()), c.y()),
				 std::max(std::max(a.z(), b.z()), c.z()));

		box.setMin(min);
		box.setMax(max);

		return true;
	}

	virtual vec3 random(const vec3& o) const {

		vec3 direction = cross(b - a, c - a);

		// normalize
		direction /= direction.length();

		onb uvw;
		uvw.buildFromNormal(direction);

		// convert random direction to sphere to local coordinates
		return uvw.local(randomPointInHemisphere());
	}
};