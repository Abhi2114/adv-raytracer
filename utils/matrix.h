#pragma once

#include <iostream>
#include "vec3.h"

class matrix {

public:

	matrix(int, int);
	matrix();
	~matrix();
	matrix(const matrix&);
	matrix& operator=(const matrix&);

	inline double& operator()(int x, int y) { return p[x][y]; }

	matrix& operator+=(const matrix&);
	matrix& operator-=(const matrix&);
	matrix& operator*=(const matrix&);
	// special op, for multiplying 4X4 matrices by 3 vectors
	vec3 operator*(const vec3&);
	matrix& operator*=(double);
	matrix& operator/=(double);
	matrix  operator^(int);

	friend std::ostream& operator<<(std::ostream&, const matrix&);
	friend std::istream& operator>>(std::istream&, matrix&);

	void swapRows(int, int);
	matrix transpose();

	static matrix createIdentity(int);
	static matrix solve(matrix, matrix);
	static matrix bandSolve(matrix, matrix, int);

	// functions on vectors
	static double dotProduct(matrix, matrix);

	// functions on augmented matrices
	static matrix augment(matrix, matrix);
	matrix gaussianEliminate();
	matrix rowReduceFromGaussian();
	void readSolutionsFromRREF(std::ostream& os);
	matrix inverse();

private:
	int rows_, cols_;
	double** p;

	void allocSpace();
	matrix expHelper(const matrix&, int);
};

matrix operator+(const matrix&, const matrix&);
matrix operator-(const matrix&, const matrix&);
matrix operator*(const matrix&, const matrix&);
matrix operator*(const matrix&, double);
matrix operator*(double, const matrix&);
matrix operator/(const matrix&, double);
