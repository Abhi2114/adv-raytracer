#pragma once

#include "vec3.h"
#include <random>
#include <limits>

#include <iostream>

extern const double MAXDOUBLE;
extern const double PI;

// some useful utility functions

// generate a uniform distribution in the given range
double genRandom(double lower, double upper);

// overload for ints
int genRandom(int lower, int upper);

// generate a random point inside a sphere of unit radius
vec3 randomPointInSphere();

// generate a random point in a hemisphere of unit radius
vec3 randomPointInHemisphere();

// shoot some rays toward a sphere
vec3 randomToSphere(double radius, double ds);

// generate a random point on a sphere of unit radius
vec3 randomPointOnSphere();

// random cosine direction
vec3 randomCosineDirection();

// standard reflection formula
vec3 reflect(const vec3& v, const vec3& normal);

// refractions!
// index = relative refractive index
bool refract(const vec3& v, const vec3& normal, double index, vec3& refracted);

// vary glass reflectivity with angle using a simple polynomial approximation
double schlick(double cosine, double index);