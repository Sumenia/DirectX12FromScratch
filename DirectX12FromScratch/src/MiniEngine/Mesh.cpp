#include "MiniEngine/Mesh.h"

using namespace MiniEngine;

Mesh::Mesh() : _isLoaded(false) {}

Mesh::~Mesh() {}

bool Mesh::loadObjFromFile(const std::string &path) {

	std::ifstream file(path);
	std::string line;

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
			_vertexs.push_back(vertex);
		}
		else if (type == "vt") {
			Vector2f textureVertex;
			in >> textureVertex.x >> textureVertex.y;
			_uvs.push_back(textureVertex);
		}
		else if (type == "vn") {
			Vector3f normal;
			in >> normal.x >> normal.y >> normal.z;
			_normals.push_back(normal);
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
			_vertexIndices.push_back(vertexIndex[0]);
			_vertexIndices.push_back(vertexIndex[1]);
			_vertexIndices.push_back(vertexIndex[2]);
			_uvIndices.push_back(uvIndex[0]);
			_uvIndices.push_back(uvIndex[1]);
			_uvIndices.push_back(uvIndex[2]);
			_normalIndices.push_back(normalIndex[0]);
			_normalIndices.push_back(normalIndex[1]);
			_normalIndices.push_back(normalIndex[2]);
		}
	}

	file.close();

	_isLoaded = true;
	return true;
}

bool Mesh::isLoaded() const {
	return _isLoaded;
}

const std::vector<unsigned int> &Mesh::getNormalIndices() const {
	return _normalIndices;
}

const std::vector<unsigned int> &Mesh::getUvIndices() const {
	return _uvIndices;
}

const std::vector<unsigned int> &Mesh::getVertexIndices() const {
	return _vertexIndices;
}

const std::vector<Vector3f> &Mesh::getVertexs() const {
	return _vertexs;
}

const std::vector<Vector2f> &Mesh::getUvs() const {
	return _uvs;
}

const std::vector<Vector3f> &Mesh::getNormals() const {
	return _normals;
}