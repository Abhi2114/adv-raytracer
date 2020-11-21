#include <iostream>
#include <limits>
#include <random>
#include <math.h>
#include <algorithm>
#include <string>

#include "utils/vec3.h"
#include "utils/matrix.h"
#include "utils/Image.h"
#include "utils/ray.h"

#include "utils/Hitable.h"
#include "objects/Sphere.h"
#include "utils/HitableList.h"
#include "utils/Camera.h"
#include "utils/mesh.h"
#include "utils/BVHNode.h"

#include "surfaces/Lambertian.h"
#include "surfaces/Metal.h"
#include "surfaces/Dielectric.h"

#include "lights/Diffuse.h"
#include "objects/AARectangle.h"
#include "objects/Triangle.h"

#include "pdfs/cosinePDF.h"
#include "pdfs/hitablePDF.h"
#include "pdfs/mixturePDF.h"

const int MAX_DEPTH = 50;

vec3 cameraEye(278, 278, -800);
vec3 lightPosition(0, 1, 0);  // point light, used for fun
vec3 specularColor(1, 1, 1);
const double specularExponent = 100.0;

// area light position, oriented along the x-z plane, constant y = k

double x0 = 150;
double x1 = 400;
double z0 = 127;
double z1 = 332;
double k = 554;

/*
double x0 = 213;
double x1 = 343;
double z0 = 227;
double z1 = 332;
double k = 554;
*/

vec3 color(const ray& r, Hitable *boxWorld, Hitable *modelWorld,
		   Hitable *lightShapes, 
		   const size_t numSources, int depth) {

	HitRecord record; // the final record to be chosen
	HitRecord boxRecord, modelRecord;

	bool isBoxHit = boxWorld->isHit(r, 0.001, MAXDOUBLE, boxRecord);
	bool isModelHit = modelWorld->isHit(r, 0.001, MAXDOUBLE, modelRecord);

	if (isBoxHit && isModelHit) {
		if (boxRecord.distance < modelRecord.distance)
			record = boxRecord;
		else
			record = modelRecord;
	}
	else if (isBoxHit)
		record = boxRecord;
	else if (isModelHit)
		record = modelRecord;

	if (isBoxHit || isModelHit) {
		// there is a hit! get the scattered ray and attenuation using the material properties of the surface
		ScatterRecord srecord;

		vec3 emit = record.material->emitted();

		if (depth < MAX_DEPTH && record.material->scatter(r, record, srecord)) {

			if (!srecord.isDiffuse) {
				// normal stuff, special case for specular surfaces
				return srecord.attenuation * color(srecord.scatteredRay, boxWorld, modelWorld, lightShapes, numSources, depth + 1);
			}
			else {

				PDF** plist = new PDF* [numSources];
				plist[0] = srecord.pdf;  // pdf for indirect lighting

				// init the pdfs for direct lighting
				for (size_t i = 1; i < numSources; ++i)
					plist[i] = new HitablePDF(record.point, lightShapes->getHitable(i - 1));

				// mix em up!!
				mixturePDF p(plist, int(numSources));

				ray scattered = ray(record.point, p.generate());
				double pdfVal = p.value(scattered.getDirection());

				// free all the pdfs!
				for (size_t i = 0; i < numSources; ++i)
					delete plist[i];

				delete[] plist;

				// compute color recursively
				vec3 col = color(scattered, boxWorld, modelWorld, lightShapes, numSources, depth + 1);

				// split into diffuse and specular
				vec3 diffuse = srecord.attenuation * col * record.material->scatteringPDF(r, record, scattered) / pdfVal;

				vec3 totalLight = diffuse;

				// clamp
				if (totalLight.x() > 1) totalLight[0] = 1;
				if (totalLight.y() > 1) totalLight[1] = 1;
				if (totalLight.z() > 1) totalLight[2] = 1;

				return totalLight;
			}
		}

		// we hit a light source, or we miss it even after MAX_DEPTH bounces in which case emit = (0, 0, 0)
		return emit;
	}

	// no sky now, paint black if miss
	return vec3(0, 0, 0);

	/*
	vec3 unitDirection = unit_vector(r.getDirection());
	// change range
	double t = 0.5 * (unitDirection.y() + 1.0);
	// interpolate colors for the sky
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	*/
}

int main() {

	int width = 800;
	int height = 800;

	// set the # of samples we want per pixel
	int numSamples = 500;

	Image image(width, height);

	// Hitable* list[5];
	// list[0] = new Sphere(vec3(-0.3, 0, 0), 0.5, new Metal(vec3(1.0, 1.0, 0.0), 0.0));
	// list[1] = new Sphere(vec3(0.0, -100.5, -1), 100, new Metal(vec3(0.1, 0.5, 0.6), 0.0));
	// list[2] = new Sphere(vec3(1, 0, 0), 0.5, new Lambertian(vec3(0.5, 0.1, 0.1)));
	// list[3] = new Sphere(vec3(-3, 0.2, 1), 0.7, new Dielectric(1.4));
	// list[4] = new Sphere(vec3(-3, 0.2, 1), -0.65, new Dielectric(1.4));
	// 
	// Hitable* world = new HitableList(list, 5);

	// Hitable* list[6];
	// 
	// list[0] = new Sphere(vec3(0, -1000, 0), 1000, new Lambertian(vec3(0.5, 0.9, 0.1)));
	// list[1] = new Sphere(vec3(2, 4, 3), 4, new Dielectric(1.4));
	// list[2] = new Sphere(vec3(2, 4, 3), -3.95, new Dielectric(1.4));
	// list[3] = new Sphere(vec3(-1, 10, 4), 3, new Diffuse(vec3(1.0, 1.0, 1.0)));
	// list[4] = new Sphere(vec3(2, 11, -3), 2, new Diffuse(vec3(1.0, 1.0, 1.0)));
	// list[5] = new XYRectangle(3, 5, 0, 6, -2, new Diffuse(vec3(1.0, 1.0, 1.0)));
	// 
	// Hitable* world = new HitableList(list, 6);

	matrix S(4, 4);

	S(0, 0) = 2000;
	S(1, 1) = 2500;
	S(2, 2) = 2000;
	S(3, 3) = 1;

	matrix T(4, 4);

	T(0, 0) = 1;
	T(1, 1) = 1;
	T(2, 2) = 1;
	T(3, 3) = 1;
	T(0, 3) = 220;
	T(1, 3) = 0;
	T(2, 3) = 400;

	matrix RZ(4, 4);
	matrix RY(4, 4);
	matrix RY1(4, 4);
	matrix RX(4, 4);
	/*
	R(0, 0) = 1;
	R(1, 2) = 1;
	R(2, 1) = -1;
	R(3, 3) = 1;
	*/

	RY(0, 0) = -1;
	RY(1, 1) = 1; // 
	RY(2, 2) = -1;
	RY(3, 3) = 1;

	RY1(0, 0) = 0.866;
	RY1(0, 2) = -0.5;
	RY1(1, 1) = 1;
	RY1(2, 0) = 0.5;
	RY1(2, 2) = 0.866;
	RY1(3, 3) = 1;

	/*
	RY1(0, 0) = 0.7071;
	RY1(0, 2) = -0.7071;
	RY1(1, 1) = 1;
	RY1(2, 0) = 0.7071;
	RY1(2, 2) = 0.7071;
	RY1(3, 3) = 1;
	*/

	RX(0, 0) = 1;
	RX(1, 1) = 0;
	RX(1, 2) = 1;
	RX(2, 1) = -1;
	RX(2, 2) = 0;
	RX(3, 3) = 1;

	RZ(0, 0) = 0;
	RZ(0, 1) = 1;
	RZ(1, 0) = -1;
	RZ(2, 2) = 1;
	RZ(3, 3) = 1;

	// RY *= RZ;
	// RX *= RY;
	RY1 *= RX;
	S *= RY1;
	T *= S;
	// R *= S;

	mesh m;
	m.loadFromObjectFile("pdfDesign3.obj", new Metal(vec3(0.0, 1.0, 1.0), 0.5), T);

	// cornell box!!
	Hitable** boxList = new Hitable*[9];
	Hitable** modelList = new Hitable*[m.triangles.size()];

	Material* red = new Lambertian(vec3(0.65, 0.05, 0.05));
	Material* white = new Lambertian(vec3(0.73, 0.73, 0.73));
	Material* green = new Lambertian(vec3(0.12, 0.45, 0.15));
	Material* checker = new Lambertian(vec3(0, 0, 0));
	Material* light = new Diffuse(vec3(1, 1, 0));
	Material* light1 = new Diffuse(vec3(1, 1, 1));

	boxList[0] = new FlipNormals(new YZRectangle(0, 555, 0, 555, 555, red));
	boxList[1] = new YZRectangle(0, 555, 0, 555, 0, green);
	// list[2] = new XZRectangle(x0, x1, z0, z1, k, light1);
	boxList[2] = new Sphere(vec3((x0 + x1) / 2 - 200, k - 70, (z0 + z1) / 2), 70, light1);
	boxList[7] = new Sphere(vec3((x0 + x1) / 2, k - 70, (z0 + z1) / 2), 70, light1);
	boxList[8] = new Sphere(vec3((x0 + x1) / 2 + 200, k - 70, (z0 + z1) / 2), 70, light1);
	boxList[3] = new FlipNormals(new XZRectangle(0, 555, 0, 555, 555, white));  // ceiling
	boxList[4] = new XZRectangle(0, 555, 0, 555, 0, checker); // floor
	boxList[5] = new FlipNormals(new XYRectangle(0, 555, 0, 555, 555, white));
	// list[6] = new Sphere(vec3(130, 100, 100), 100, new Dielectric(1.5));
	boxList[6] = new Sphere(vec3(130, 100, 300), 100, new Metal(vec3(0.5, 0.9, 0.1), 0.0));
	// list[7] = new Triangle(vec3(200, 0, 600), vec3(350, 300, 295), vec3(500, 0, 200), new Metal(vec3(0.5, 0.9, 0.1), 0.0));
	// list[6] = new Sphere(vec3(130, 100, 100), 100, new Lambertian(vec3(0.3, 0.1, 0.9)));
	// list[6] = new Sphere(vec3(130, 100, 100), 100, light1);
	// list[7] = new Sphere(vec3(400, 120, 295), 120, new Lambertian(vec3(0.2, 0.7, 0.2)));
	
	for (size_t i = 0; i < m.triangles.size(); ++i)
		modelList[i] = &m.triangles[i];

	Hitable* modelWorld = new BVHNode(modelList, m.triangles.size());
	Hitable* boxWorld = new HitableList(boxList, 9);

	vec3 lookAt = vec3(278, 278, 0);
	double distToFocus = 10.0;
	double aperture = 0.0;
	double vfov = 40.0;

	// setup the camera
	// Camera camera(cameraEye, lookAt, vec3(0, 1, 0), 100.0, double(width) / double(height), 1.0, (cameraEye - lookAt).length());
	// Camera camera(cameraEye, lookAt, vec3(0, 1, 0), 120.0, double(width) / double(height), 0.0, 1);

	Camera camera(cameraEye, lookAt, vec3(0, 1, 0), vfov, double(width) / double(height), aperture, distToFocus);

	Hitable *glassBall = new Sphere(vec3(130, 100, 100), 100, nullptr);

	Hitable* plist[3];  // list of pdfs for direct lighting
	plist[0] = boxList[2];
	plist[1] = boxList[7];
	plist[2] = boxList[8];

	Hitable* lightShapes = new HitableList(plist, 3);

	double percent = 0;  // percent complete so far

	// fill in the image
	for (int j = height - 1; j > -1; --j) {
		for (int i = 0; i < width; ++i) {

			vec3 col(0, 0, 0);

			for (int n = 0; n < numSamples; ++n) {
				// randomly perturb the ray within the "neighborhood" of the pixel center
				double u = double(i + genRandom(0.0, 1.0)) / double(width);
				double v = double(j + genRandom(0.0, 1.0)) / double(height);
				ray r = camera.getRay(u, v);
				col += color(r, boxWorld, modelWorld, lightShapes, 4, 0);  // always 1 more source
			}

			// average the computed color
			col /= numSamples;

			col = vec3(sqrt(col.r()), sqrt(col.g()), sqrt(col.b()));

			image.set(i, height - j - 1, col);  // flip
		}

		if ((double(height) - double(j)) / double(height) * 100 - percent >= 1) {
			percent++;
			std::cout << percent << "% complete\n";
		}
	}

	// save
	image.write();
	
	return 0;
}