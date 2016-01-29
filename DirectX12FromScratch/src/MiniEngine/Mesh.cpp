#include "MiniEngine/Mesh.h"

using namespace MiniEngine;

Mesh::Mesh() {}

Mesh::~Mesh() {}

bool Mesh::loadObjFromFile(const std::string &path) {

	std::ifstream file(path);
	std::string line;

	std::vector<Vector3f> temp_vertices;
	std::vector<Vector2f> temp_uvs;
	std::vector<Vector3f> temp_normals;

	if (!file.is_open()) {
		std::cout << "ERROR" << std::endl;
		return false;
	}

	for (std::string line; std::getline(file, line); ) {
		
		std::istringstream in(line);
		std::string type;
		in >> type;

		if (type == "v") {
			Vector3f vertex;
			in >> vertex.x >> vertex.y >> vertex.z;
			temp_vertices.push_back(vertex);
		}
		else if (type == "vt") {
			Vector2f textureVertex;
			in >> textureVertex.x >> textureVertex.y;
			temp_uvs.push_back(textureVertex);
		}
		else if (type == "vn") {
			Vector3f normal;
			in >> normal.x >> normal.y >> normal.z;
			temp_normals.push_back(normal);
		}
		else if (type == "f") {

			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			std::string substring;

			for (int i = 0; i < 3; ++i) {
				in >> substring;
				std::stringstream ss(substring);
				ss >> vertexIndex[i];
				ss.ignore();
				ss >> uvIndex[i];
				ss.ignore();
				ss >> normalIndex[i];
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	file.close();
	
	for (unsigned int i = 0; i < vertexIndices.size(); ++i) {
		unsigned int vertexIndex = vertexIndices[i];
		Vector3f vertex = temp_vertices[vertexIndex - 1];
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < uvIndices.size(); ++i) {
		unsigned int uvIndex = uvIndices[i];
		Vector2f uv = temp_uvs[uvIndex - 1];
		uvs.push_back(uv);
	}

	for (unsigned int i = 0; i < normalIndices.size(); ++i) {
		unsigned int normalIndex = normalIndices[i];
		Vector3f normal = temp_normals[normalIndex - 1];
		normals.push_back(normal);
	}
	
	temp_uvs.clear();
	temp_vertices.clear();
	temp_normals.clear();

	/*for (int i = 0; i < vertices.size(); ++i) {
		std::cout << "VERTICES : " << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
	}

	for (int i = 0; i < uvs.size(); ++i) {
		std::cout << "UVS : " << uvs[i].x << " " << uvs[i].y << std::endl;
	}

	for (int i = 0; i < normals.size(); ++i) {
		std::cout << "NORMALS : " << normals[i].x << " " << normals[i].y << " " << normals[i].z << std::endl;
	} */

	std::cout << "DONE" << std::endl;

	return true;
}