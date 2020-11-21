#pragma once

#include "..//utils/Hitable.h"
#include "..//utils/Material.h"

// aligned with the XY plane
class XYRectangle : public Hitable {

private:

	// define a plane on which this rectangle resides
	double x0, x1;
	double y0, y1;
	double k;
	Material* material;

public:

	XYRectangle(double x0, double x1, double y0, double y1, double k, Material *material) :
		x0(x0), x1(x1),
		y0(y0), y1(y1),
		k(k), material(material) {}

	virtual bool isHit(const ray& r, double tMin, double tMax, HitRecord& record) const {

		// determine t, the distance of the intersection point, using the z-coordinate
		double t = (k - r.getOrigin().z()) / (r.getDirection().z());

		// check range
		if (t < tMin || t > tMax) return false;

		// compute x and y using t, the intersection point with the plane is (x, y, k)
		double x = r.getOrigin().x() + t * r.getDirection().x();
		double y = r.getOrigin().y() + t * r.getDirection().y();

		// check if this point is within the bounds of the rectangle defined
		if (x < x0 || x > x1 || y < y0 || y > y1) return false;

		// else, we have a hit
		record.distance = t;
		record.normal = vec3(0, 0, 1);  // AA rectangle along the XY plane will always have the same normal
		record.point = r.pointAtParameter(t);
		record.material = material;

		return true;
	}

	virtual bool makeBB(AABB& box) const {

		box.setMin(vec3(x0, y0, k));
		box.setMax(vec3(x1, y1, k));
		return true;
	}
};

// aligned with the XZ plane
// used for the light source
class XZRectangle : public Hitable {

private:

	double x0, x1;
	double z0, z1;
	double k;
	Material* material;

public:

	XZRectangle(double x0, double x1, double z0, double z1, double k, Material* material) :
				x0(x0), x1(x1),
				z0(z0), z1(z1),
				k(k), material(material) {}

	virtual bool isHit(const ray& r, double tMin, double tMax, HitRecord& record) const {

		// determine t, the distance of the intersection point, using the y-coordinate
		double t = (k - r.getOrigin().y()) / (r.getDirection().y());

		// check range
		if (t < tMin || t > tMax) return false;

		// compute x and y using t, the intersection point with the plane is (x, k, z)
		double x = r.getOrigin().x() + t * r.getDirection().x();
		double z = r.getOrigin().z() + t * r.getDirection().z();

		// check if this point is within the bounds of the rectangle defined
		if (x < x0 || x > x1 || z < z0 || z > z1) return false;

		// else, we have a hit
		record.distance = t;
		record.normal = vec3(0, 1, 0);  // AA rectangle along the XZ plane will always have the same normal
		record.point = r.pointAtParameter(t);
		record.material = material;

		return true;
	}

	virtual bool makeBB(AABB& box) const {

		box.setMin(vec3(x0, k, z0));
		box.setMax(vec3(x1, k, z1));
		return true;
	}

	// sampling the light source directly, compute pdf using the inverse square law
	virtual double pdfValue(const vec3& o, const vec3& v) const {
		
		// see if this (o, v) pair can hit the light source
		HitRecord record;

		if (isHit(ray(o, v), 0.001, MAXDOUBLE, record)) {

			double area = (x1 - x0) * (z1 - z0);
			double ds = v.length_squared();

			double cosine = dot(unit_vector(-v), -record.normal);

			return ds / (cosine * area);
		}

		return 0;
	}

	// generate a random direction on the light source
	virtual vec3 random(const vec3& o) const { 
		
		// random point on the light source r
		vec3 r(x0 + genRandom(0, 1) * (x1 - x0), k, z0 + genRandom(0, 1) * (z1 - z0));

		return r - o;
	}
};

// aligned with the YZ plane
class YZRectangle : public Hitable {

private:

	double y0, y1;
	double z0, z1;
	double k;
	Material* material;

public:

	YZRectangle(double y0, double y1, double z0, double z1, double k, Material* material) :
				y0(y0), y1(y1),
				z0(z0), z1(z1),
				k(k), material(material) {}

	virtual bool isHit(const ray& r, double tMin, double tMax, HitRecord& record) const {

		// determine t, the distance of the intersection point, using the x-coordinate
		double t = (k - r.getOrigin().x()) / (r.getDirection().x());

		// check range
		if (t < tMin || t > tMax) return false;

		// compute x and y using t, the intersection point with the plane is (k, y, z)
		double y = r.getOrigin().y() + t * r.getDirection().y();
		double z = r.getOrigin().z() + t * r.getDirection().z();

		// check if this point is within the bounds of the rectangle defined
		if (y < y0 || y > y1 || z < z0 || z > z1) return false;

		// else, we have a hit
		record.distance = t;
		record.normal = vec3(1, 0, 0);  // AA rectangle along the YZ plane will always have the same normal
		record.point = r.pointAtParameter(t);
		record.material = material;

		return true;
	}

	virtual bool makeBB(AABB& box) const {

		box.setMin(vec3(k, y0, z0));
		box.setMax(vec3(k, y1, z1));
		return true;
	}
};