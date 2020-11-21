#pragma once

// axis-aligned bounding boxes - the coolest things in graphics!!

#include <algorithm>
#include "vec3.h"
#include "ray.h"

class AABB {

private:

	// the llc and urc of a bounding box
	vec3 _min, _max;

public:

	AABB() {}

	AABB(const vec3 &_min, const vec3 &_max) : _min(_min), _max(_max) {}

	vec3 min() const { return _min; }
	vec3 max() const { return _max; }

	void setMin(const vec3& min) { _min = min; }
	void setMax(const vec3& max) { _max = max; }

	// test hit
	bool isHit(const ray& r, double tmin, double tmax) const {

		for (int i = 0; i < 3; ++i) {

			double invD = 1.0f / r.getDirection()[i];
			double t0 = (min()[i] - r.getOrigin()[i]) * invD;
			double t1 = (max()[i] - r.getOrigin()[i]) * invD;

			if (invD < 0.0f) std::swap(t0, t1);

			tmin = t0 > tmin ? t0 : tmin;
			tmax = t1 < tmax ? t1 : tmax;

			if (tmax <= tmin) return false;
		}

		return true;
	}

	// merge 2 bounding boxes together
	friend AABB combineBoxes(const AABB& box0, const AABB& box1) {

		vec3 small(std::min(box0.min().x(), box1.min().x()),
				   std::min(box0.min().y(), box1.min().y()),
				   std::min(box0.min().z(), box1.min().z()));

		vec3 big(std::max(box0.max().x(), box1.max().x()),
				 std::max(box0.max().y(), box1.max().y()),
				 std::max(box0.max().z(), box1.max().z()));

		return AABB(small, big);
	}
};