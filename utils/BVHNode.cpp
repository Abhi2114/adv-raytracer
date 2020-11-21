#include "BVHNode.h"
#include "funcs.h"
#include "HitableList.h"

// some comparator functions
int xCompare(const void* a, const void* b) {

	AABB leftBox, rightBox;

	Hitable* ah = *(Hitable**)a;
	Hitable* bh = *(Hitable**)b;

	if (!ah->makeBB(leftBox) || !bh->makeBB(rightBox))
		std::cerr << "no bounding box in bvh constructor\n";

	if (leftBox.min().x() < rightBox.min().x())
		return -1;

	return 1;
}

int yCompare(const void* a, const void* b) {

	AABB leftBox, rightBox;

	Hitable* ah = *(Hitable**)a;
	Hitable* bh = *(Hitable**)b;

	if (!ah->makeBB(leftBox) || !bh->makeBB(rightBox))
		std::cerr << "no bounding box in bvh constructor\n";

	if (leftBox.min().y() < rightBox.min().y())
		return -1;

	return 1;
}

int zCompare(const void* a, const void* b) {

	AABB leftBox, rightBox;

	Hitable* ah = *(Hitable**)a;
	Hitable* bh = *(Hitable**)b;

	if (!ah->makeBB(leftBox) || !bh->makeBB(rightBox))
		std::cerr << "no bounding box in bvh constructor\n";

	if (leftBox.min().z() < rightBox.min().z())
		return -1;

	return 1;
}

BVHNode::BVHNode(Hitable** list, size_t n) {

	// the actual construction of the tree

	// choose a random axis to sort on everytime
	int axis = int(3 * genRandom(0.0, 1.0));

	if (axis == 0) // x
		qsort(list, n, sizeof(Hitable*), xCompare);
	else if (axis == 1) // y
		qsort(list, n, sizeof(Hitable*), yCompare);
	else  // z
		qsort(list, n, sizeof(Hitable*), zCompare);
	
	
	/*
	if (n <= 100) {
		left = new HitableList(list, n / 2);
		right = new HitableList(list + n / 2, n / 2);
	}
	*/
	if (n == 1)
		left = right = list[0];
	else if (n == 2) {
		left = list[0];
		right = list[1];
	}
	else {
		// recurse
		left = new BVHNode(list, n / 2);
		right = new BVHNode(list + n / 2, n - n / 2);
	}
	// merge the left and right boxes together
	AABB leftBox, rightBox;

	if (!left->makeBB(leftBox) || !right->makeBB(rightBox))
		std::cerr << "no bounding box in bvh constructor\n";

	box = combineBoxes(leftBox, rightBox);
}

bool BVHNode::isHit(const ray& r, double tMin, double tMax, HitRecord& record) const {

	if (box.isHit(r, tMin, tMax)) {
		// test where to go next
		HitRecord leftRecord, rightRecord;

		bool hitLeft = left->isHit(r, tMin, tMax, leftRecord);
		bool hitRight = right->isHit(r, tMin, tMax, rightRecord);

		if (hitLeft && hitRight) {
			// overlap, decide based on proximity
			if (leftRecord.distance < rightRecord.distance)
				record = leftRecord;
			else
				record = rightRecord;

			return true;
		}
		else if (hitLeft) {
			record = leftRecord;
			return true;
		}
		else if (hitRight) {
			record = rightRecord;
			return true;
		}
	}

	return false;
}

bool BVHNode::makeBB(AABB& b) const {
	b = box;
	return true;
}
