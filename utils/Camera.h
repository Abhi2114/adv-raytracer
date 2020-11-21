#pragma once

// creds: Peter Shirley, Ray Tracing in a Weekend

#include "ray.h"

#include "funcs.h"

// a simple camera class

class Camera {

private:

	vec3 origin, lowerLeftCorner, horizontal, vertical;
	vec3 u, v, w;  // orthonormal basis
	double lensRadius;  // radius of the camera lens

public:

	// the image plane is at z = -1 by default
	Camera() {
		
		origin = vec3(0, 0, 0);
		// the image plane is defined by the following 3 parameters
		horizontal = vec3(4, 0, 0);
		vertical = vec3(0, 2, 0);
		lowerLeftCorner = vec3(-2, -1, -1);
		lensRadius = 0.0;
	}

	// vertical field of view
	// also set the camera co-ordinate frame
	Camera(const vec3& lookfrom, 
		   const vec3& lookat,
		   const vec3& up,
		   double fovy, 
		   double aspect,
		   double aperture,
		   double focusDistance) {

		lensRadius = aperture / 2.0;

		// convert to radians
		fovy *= PI / 180.0;

		// compute the origin of the image plane in 3D space
		double halfHeight = tan(fovy / 2.0);
		// use the aspect ratio to compute half width
		double halfWidth = aspect * halfHeight;

		origin = lookfrom;

		// set up the orthonormal basis for the camera, still maintaining the right handedness of the system
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(up, w));
		v = cross(w, u);

		// push/pull everything away/back by a multiple of the focus distance
		lowerLeftCorner = origin - focusDistance * (halfWidth * u + halfHeight * v + w);

		horizontal = 2 * halfWidth * focusDistance * u;
		vertical = 2 * halfHeight * focusDistance * v;
	}

	// get ray being shot at the image plane
	ray getRay(double s, double t) { 

		// choose a random point inside the sphere as the camera eye
		vec3 rm = lensRadius * randomPointInSphere();
		// scale along the horizontal and vertical directions of the camera coordinate frame
		vec3 offset = rm.x() * u + rm.y() * v;
		// compute new ray/camera origin
		vec3 newOrigin = origin + offset;

		return ray(newOrigin, lowerLeftCorner + s * horizontal + t * vertical - newOrigin); 
	}
};
