#include "funcs.h"

const double MAXDOUBLE = std::numeric_limits<double>::max();
const double PI = 3.14159265359;


// some useful utility functions

// generate a uniform distribution in the given range
double genRandom(double lower, double upper) {

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(lower, upper);

	return dis(gen);
}

// overload for ints
int genRandom(int lower, int upper) {

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(lower, upper);

	return dis(gen);
}

// generate a random point inside a sphere of unit radius
vec3 randomPointInSphere() {

	/*
	// using the parametric form

	// generate random values for the parameters

	double longitude = genRandom(0.0, 2.0 * PI);
	double colatitude = genRandom(0.0, PI / 2.0);

	double rho = genRandom(0.0, 1.0);

	double x = rho * cos(longitude) * sin(colatitude);
	double y = rho * sin(longitude) * sin(colatitude);
	double z = rho * cos(colatitude);

	return vec3(x, y, z);
	*/

	// using rejection sampling

	vec3 p;

	do {
		p = 2 * vec3(genRandom(0.0, 1.0), genRandom(0.0, 1.0), genRandom(0.0, 1.0)) - vec3(1.0, 1.0, 1.0);
	} while (dot(p, p) >= 1.0);

	// point generated needs to be on the periphery
	return p;
}

// generate a random point in a hemisphere of unit radius
vec3 randomPointInHemisphere() {

	// using the parametric form

	// generate random values for the parameters
	double longitude = genRandom(0.0, 2.0 * PI);
	double colatitude = genRandom(0.0, PI / 2.0);

	double rho = genRandom(0.0, 1.0);

	double x = rho * cos(longitude) * sin(colatitude);
	double y = rho * sin(longitude) * sin(colatitude);
	double z = rho * cos(colatitude);

	return vec3(x, y, z);
}

// shoot some rays toward a sphere
vec3 randomToSphere(double radius, double ds) {

	double r1 = genRandom(0.0, 1.0);
	double r2 = genRandom(0.0, 1.0);

	double z = 1 + r2 * (sqrt(1 - radius * radius / ds) - 1);
	double phi = 2 * PI * r1;

	double x = cos(phi) * sqrt(1 - z * z);
	double y = sin(phi) * sqrt(1 - z * z);

	return vec3(x, y, z);
}

// generate a random point on a sphere of unit radius
vec3 randomPointOnSphere() {

	// using rejection sampling

	vec3 p;

	do {
		p = 2 * vec3(genRandom(0.0, 1.0), genRandom(0.0, 1.0), genRandom(0.0, 1.0)) - vec3(1.0, 1.0, 1.0);
	} while (dot(p, p) >= 1.0);

	// point generated needs to be on the periphery
	return unit_vector(p);
}

// random cosine direction
vec3 randomCosineDirection() {

	// generate random directions in a sphere using 2 random nums, r1 and r2, rotationally symmetric about the z-axis
	double r1 = genRandom(0.0, 1.0);
	double r2 = genRandom(0.0, 1.0);

	double z = sqrt(1 - r2);

	double phi = 2 * PI * r1;
	double x = cos(phi) * 2 * sqrt(r2);
	double y = sin(phi) * 2 * sqrt(r2);

	return vec3(x, y, z);
}

// standard reflection formula
vec3 reflect(const vec3& v, const vec3& normal) {

	return v - 2 * dot(v, normal) * normal;
}

// refractions!
// index = relative refractive index
bool refract(const vec3& v, const vec3& normal, double index, vec3& refracted) {

	vec3 nv = unit_vector(v);
	double d = dot(nv, normal);

	double discriminant = 1.0 - index * index * (1 - d * d);

	if (discriminant > 0) {
		// set the refracted ray
		refracted = index * (nv - d * normal) - sqrt(discriminant) * normal;
		return true;
	}

	// no refraction possible
	return false;
}

// vary glass reflectivity with angle using a simple polynomial approximation
double schlick(double cosine, double index) {

	double r1 = (1 - index) / (1 + index);
	return r1 * r1 + (1 - r1 * r1) * pow(1 - cosine, 5);
}