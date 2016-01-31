#include "MiniEngine/Mesh.h"

using namespace MiniEngine;

bool Vertex::operator==(const Vertex& toCompare) const {
	return (toCompare.normal.x == normal.x &&
		toCompare.normal.y == normal.y &&
		toCompare.normal.z == normal.z &&
		toCompare.uv.x == uv.x &&
		toCompare.uv.y == uv.y &&
		toCompare.vertice.x == vertice.x &&
		toCompare.vertice.y == vertice.y &&
		toCompare.vertice.z == vertice.z);
}

Mesh::Mesh() : _isLoaded(false) {}

Mesh::~Mesh() {}

void Mesh::replaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}

bool Mesh::loadObjFromFile(const std::string &path) {

	std::ifstream file(path);
	std::string line;

	if (!file.is_open()) {
		std::cout << "ERROR" << std::endl;
		return false;
	}

	_vertexs.clear();
	_indices.clear();

	for (std::string line; std::getline(file, line); ) {
		
		std::istringstream in(line);
		std::string type;
		in >> type;

		if (type == "v") {
			Vector3f vertex;
			in >> vertex.x >> vertex.y >> vertex.z;
			_vertices.push_back(vertex);
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
			unsigned int tmp;
			std::string substring;

			for (int i = 0; i < 3; ++i) {
				in >> substring;
				replaceAll(substring, "/", " ");

				std::stringstream ss(substring);
				ss >> vertexIndex[i];
				ss >> uvIndex[i];
				
				tmp = 0;
				ss >> tmp;

				if (tmp > 0) {
					normalIndex[i] = tmp;
				} else {
					normalIndex[i] = uvIndex[i];
					uvIndex[i] = 0;
				}
			}
			_verticesIndices.push_back(vertexIndex[0]);
			_verticesIndices.push_back(vertexIndex[1]);
			_verticesIndices.push_back(vertexIndex[2]);
			_uvIndices.push_back(uvIndex[0]);
			_uvIndices.push_back(uvIndex[1]);
			_uvIndices.push_back(uvIndex[2]);
			_normalIndices.push_back(normalIndex[0]);
			_normalIndices.push_back(normalIndex[1]);
			_normalIndices.push_back(normalIndex[2]);
		}
	}

	file.close();

	for (unsigned int i = 0; i < _verticesIndices.size(); ++i) {

		Vertex vertex;

		unsigned int verticeIndex = _verticesIndices[i];
		unsigned int uvIndex = _uvIndices[i];
		unsigned int normalIndex = _normalIndices[i];

		vertex.vertice = _vertices[verticeIndex - 1];
		vertex.normal = _normals[normalIndex - 1];

		if (uvIndex > 0) {
			vertex.uv = _uvs[uvIndex - 1];
		} else {
			Vector2f zero;
			zero.x = 0;
			zero.y = 0;
			vertex.uv = zero;
		}
		
		std::vector<Vertex>::iterator it;

		it = std::find(_vertexs.begin(), _vertexs.end(), vertex);
		if (it == _vertexs.end()) {
			_vertexs.push_back(vertex);
		} else {
			unsigned int idx = it - _vertexs.begin();
			_indices.push_back(idx);
		}	
	}

	_verticesIndices.clear();
	_uvIndices.clear();
	_normalIndices.clear();
	_vertices.clear();
	_uvs.clear();
	_normals.clear();

	/* for (unsigned int i = 0; i < _vertexs.size(); ++i) {
		std::cout << "VERTEX : POSITION : " << _vertexs[i].vertice.x << " " << _vertexs[i].vertice.y << " " << _vertexs[i].vertice.z << " NORMAL : " << _vertexs[i].normal.x << " " << _vertexs[i].normal.y << " " << _vertexs[i].normal.z << std::endl;
	} */

	_isLoaded = true;
	return true;
}

bool Mesh::isLoaded() const {
	return _isLoaded;
}

const std::vector<Vertex> &Mesh::getVertexs() const {
	return _vertexs;
}

const std::vector<unsigned int> &Mesh::getIndices() const {
	return _indices;
}