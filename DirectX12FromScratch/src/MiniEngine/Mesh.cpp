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

void split(const std::string& s, char delim, std::vector<std::string>& v) {
    auto i = 0;
    auto pos = s.find(delim);
    while (pos != std::string::npos) {
        v.push_back(s.substr(i, pos - i));
        i = ++pos;
        pos = s.find(delim, pos);

        if (pos == std::string::npos)
            v.push_back(s.substr(i, s.length()));
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

			int vertexIndex[3], uvIndex[3], normalIndex[3];
			int                 tmp;
			bool			    neg;
			std::string     substring;
            std::vector<std::string>    tokens;

			for (int i = 0; i < 3; ++i)
			{
				in >> substring;

                tokens.clear();
                split(substring, '/', tokens);

                if (tokens.size() >= 1)
                {
                    std::stringstream ss(tokens[0]);
                    vertexIndex[i] = 0;
                    ss >> vertexIndex[i];

                    if (vertexIndex[i] < 0)
                        vertexIndex[i] = _vertices.size() - vertexIndex[i];
                    else if (vertexIndex[i] == 0)
                        vertexIndex[i] = -1;
                }
                else
                    vertexIndex[i] = -1;

                if (tokens.size() >= 2)
                {
                    std::stringstream ss(tokens[1]);
                    uvIndex[i] = 0;
                    ss >> uvIndex[i];

                    if (uvIndex[i] < 0)
                        uvIndex[i] = _uvs.size() - uvIndex[i];
                    else if (uvIndex[i] == 0)
                        uvIndex[i] = -1;
                }
                else
                    uvIndex[i] = -1;

                if (tokens.size() >= 3)
                {
                    std::stringstream ss(tokens[2]);
                    normalIndex[i] = 0;
                    ss >> normalIndex[i];

                    if (normalIndex[i] < 0)
                        normalIndex[i] = _normals.size() - normalIndex[i];
                    else if (normalIndex[i] == 0)
                        normalIndex[i] = -1;
                }
                else
                    normalIndex[i] = -1;
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

    Vector2f zero2;
    zero2.x = 0;
    zero2.y = 0;

    Vector3f zero3;
    zero3.x = 0;
    zero3.y = 0;
    zero3.z = 0;

	for (unsigned int i = 0; i < _verticesIndices.size(); ++i) {

		Vertex vertex;

		int verticeIndex = _verticesIndices[i];
		int uvIndex = _uvIndices[i];
		int normalIndex = _normalIndices[i];

		//std::cout << "Iteration numero  : " << i << std::endl;

		//std::cout << "_verticesIndices.size() : " << _verticesIndices.size() << std::endl;
		//std::cout << "_uvIndices.size() : " << _uvIndices.size() << std::endl;
		//std::cout << "_normalIndices.size() : " << _normalIndices.size() << std::endl;


		//std::cout << "verticeIndex : " << verticeIndex << std::endl;
		//std::cout << "uvIndex : " << uvIndex << std::endl;
		//std::cout << "normalIndex : " << normalIndex << std::endl;

		//std::cout << "_vertices.size() : " << _vertexs.size() << std::endl;
		//std::cout << "_normals.size() : " << _normals.size() << std::endl;

        if (verticeIndex >= 0)
            vertex.vertice = _vertices[verticeIndex - 1 <= 0 ? 0 : verticeIndex - 1];
        else
            vertex.vertice = zero3;

        if (normalIndex >= 0)
            vertex.normal = _normals[normalIndex - 1 <= 0 ? 0 : normalIndex - 1];
        else
            vertex.normal = zero3;

        if (uvIndex >= 0)
            vertex.uv = _uvs[uvIndex - 1 <= 0 ? 0 : uvIndex - 1];
        else
            vertex.uv = zero2;
		
		std::vector<Vertex>::iterator it;

		it = std::find(_vertexs.begin(), _vertexs.end(), vertex);
		if (it == _vertexs.end()) {
			_vertexs.push_back(vertex);
            _indices.push_back(_vertexs.size() - 1);
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