#pragma once

#include "AABB.h"
#include "Hitable.h"

// A node of a bounding volume hierarchy

class BVHNode : public Hitable {

private:

	// a box and 2 pointers
	AABB box;
	Hitable* left, * right;

public:

	// build the tree
	BVHNode(Hitable** list, size_t n);

	virtual bool isHit(const ray& r, double tMin, double tMax, HitRecord& record) const;
	virtual bool makeBB(AABB& b) const;
};