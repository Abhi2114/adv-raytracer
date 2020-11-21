#pragma once

#include "..//utils/pdf.h"
#include "..//utils/funcs.h"

// combine two pdfs to form another one

class mixturePDF : public PDF {

private:

	PDF** list;
	int numPdfs;

public:

	mixturePDF(PDF **list, int numPdfs) : list(list), numPdfs(numPdfs) {}

	virtual double value(const vec3& direction) const {

		// return a weighted average, equal for all pdfs
		double weight = 1.0 / double(numPdfs);

		double pdfVal = 0.0;

		for (int i = 0; i < numPdfs; ++i)
			pdfVal += (weight * list[i]->value(direction));

		return pdfVal;
	}

	virtual vec3 generate() const {

		int index = genRandom(0, numPdfs - 1);

		return list[index]->generate();
	}
};