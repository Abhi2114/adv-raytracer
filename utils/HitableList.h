#pragma once

#include "Hitable.h"

#include <array>

// make a collection of objects hitable
class HitableList : public Hitable {

private:

	Hitable** list;
	size_t numObjects;

public:

	HitableList() : list(nullptr), numObjects(0) {}

	HitableList(Hitable** l, size_t n) : list(l), numObjects(n) {}

	virtual Hitable* getHitable (size_t index) {

		if (index < 0 || index > numObjects - 1) return nullptr;

		return list[index];
	}

	size_t getSize() const { return numObjects; }

	virtual bool isHit(const ray& r, double tMin, double tMax, HitRecord& record) const {

		HitRecord bestRecord;
		bool hasHitAnything = false;
		double closestDistance = tMax;

		// iterate over all objects and return the hit with the closest one
		for (size_t i = 0; i < numObjects; ++i) {

			if (list[i]->isHit(r, tMin, closestDistance, bestRecord)) {
				hasHitAnything = true;
				closestDistance = bestRecord.distance;
			}
		}

		// copy
		record = bestRecord;

		return hasHitAnything;
	}

	virtual bool makeBB(AABB& box) const {

		if (numObjects < 1) return false;

		AABB temp;

		if (!list[0]->makeBB(temp)) return false;

		box = temp;

		// merge
		for (size_t i = 1; i < numObjects; ++i) {
			if (list[i]->makeBB(temp))
				box = combineBoxes(box, temp);
			else
				return false;
		}

		return true;
	}
};
