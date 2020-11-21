#pragma once

#include "..//objects/Triangle.h"
#include "matrix.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <strstream>
#include <limits>

using namespace std;

void readVector(std::string& line, vec3& v) {

	string delimiter = " ";

	size_t pos = 0;
	string token;

	string tokens[4];
	int t = 0; // index for tokens

	while ((pos = line.find(delimiter)) != string::npos) {
		// some modern C++ string magic
		token = line.substr(0, pos);
		tokens[t++] = token;
		line.erase(0, pos + delimiter.length());
	}

	tokens[t] = line;

	// we care about tokens[1..3]
	for (int i = 1; i < 4; ++i) {
		stringstream vss(tokens[i]);
		vss >> v[i - 1];
	}
}

// a mesh of triangles
struct mesh {

	std::vector<Triangle> triangles;

	// a simple file parser to load in the mesh with the appropriate material
	bool loadFromObjectFile(string fileName, Material *material, matrix& M) {

		ifstream f(fileName);

		if (!f.is_open())
			return false;

		// load all the vertices in
		vector<vec3> positions;
		vector<vec3> normals;

		while (!f.eof()) {

			char line[128];
			f.getline(line, 128);

			stringstream stream;
			stream << line;

			// s, v, f or vn

			if (line[0] == 'v') {

				std::string sline(line);

				if (line[1] == 'n') {
					// vertex normal
					vec3 normal;
					readVector(sline, normal);
					normals.push_back(normal);
				}
				else {
					// vertex position
					vec3 vertex;
					readVector(sline, vertex);
					positions.push_back(vertex);
				}
			}
			else if (line[0] == 'f') {

				string faces[3];

				stream >> line[0] >> faces[0] >> faces[1] >> faces[2];
				// split faces up
				// pos//norm

				// read the position and normal index in the vectors
				int positionIndex[3], normalIndex[3];

				for (int i = 0; i < 3; ++i) {
					// read position//normal
					string face = faces[i];
					string position = face.substr(0, face.find("/"));
					// get the remaining portion for the normal
					string normal = face.substr(position.length() + 1);  // since 2 is the size of the delimiter

					// store in array after converting
					stringstream p(position);
					p >> positionIndex[i];

					stringstream n(normal);
					n >> normalIndex[i];
				}

				// per vertex transformation
				vec3 a = positions[positionIndex[0] - 1];
				vec3 b = positions[positionIndex[1] - 1];
				vec3 c = positions[positionIndex[2] - 1];

				// special transformation for the normals
				matrix N = M.inverse().transpose();

				vec3 na = normals[normalIndex[0] - 1];
				vec3 nb = normals[normalIndex[1] - 1];
				vec3 nc = normals[normalIndex[2] - 1];

				// construct triangle
				Triangle triangle(M * a, M * b, M * c,
								  N * na, N * nb, N * nc, 
					              material);

				triangles.push_back(triangle);  // store triangle in mesh

				/*
				// face
				int faces[3];

				stream >> line[0] >> faces[0] >> faces[1] >> faces[2];

				// construct triangle
				Triangle triangle(M * positions[faces[0] - 1], M * positions[faces[1] - 1], M * positions[faces[2] - 1], material);

				triangles.push_back(triangle);  // store triangle in mesh
				*/
			}
		}

		std::cout << "num tris = " << triangles.size() << "\n";

		return true;
	}
};