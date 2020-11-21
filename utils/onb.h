#pragma once

// An orthonormal basis class

#include "vec3.h"
#include "funcs.h"

class onb {

private:

	// the new basis
	vec3 u, v, w;

public:

	onb() {}

	vec3 U() const { return u; }
	vec3 V() const { return v; }
	vec3 W() const { return w; }

	// perform a change of basis operation
	vec3 local(double x, double y, double z) const { return x * u + y * v + z * w; }
	vec3 local(const vec3& p) const { return p.x() * u + p.y() * v + p.z() * w; }

	// build an orthonormal basis from a given vector
	void buildFromNormal(const vec3& normal) {

		w = unit_vector(normal);

		vec3 a;

		if (fabs(w.x()) > 0.9)
			a = vec3(0, 1, 0);
		else
			a = vec3(1, 0, 0);

		// now safely compute the rest of the basis
		v = unit_vector(cross(w, a));
		u = cross(w, v);
	}
};