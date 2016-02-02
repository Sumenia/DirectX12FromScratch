#include "MiniEngine/Mesh.h"
#include "windows.h"

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

Mesh::Mesh() {}

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

bool Mesh::loadFromAssimp(aiMesh *mesh) {

	
		

	std::cout << "MESH" << std::endl;
	for (int j = 0; j < mesh->mNumVertices; j++)
	{
		Vertex vertex;
		Vector3f vertice;
		Vector2f uv;
		Vector3f normal;

		vertice.x = mesh->mVertices[j].x;
		vertice.y = mesh->mVertices[j].y;
		vertice.z = mesh->mVertices[j].z;

		if (mesh->HasNormals())
		{
			normal.x = mesh->mNormals[j].x;
			normal.y = mesh->mNormals[j].y;
			normal.z = mesh->mNormals[j].z;
		}
		else
			normal = Vector3f(1.0f, 1.0f, 1.0f);

		if (mesh->HasTextureCoords(0))
		{
			uv.x = mesh->mTextureCoords[0][j].x;
			uv.y = mesh->mTextureCoords[0][j].y;
		}
		else
			uv = Vector2f(0, 0);

		vertex.vertice = vertice;
		vertex.normal = normal;
		vertex.uv = uv;

		_vertexs.push_back(vertex);
	}

	for (int j = 0; j < mesh->mNumFaces; j++)
	{
		auto &&face = mesh->mFaces[j];
		for (int k = 0; k < 3; k++)
		{
			_indices.push_back(face.mIndices[k]);
		}
	}

	return true;
}

const std::vector<Vertex> &Mesh::getVertexs() const {
	return _vertexs;
}

const std::vector<unsigned int> &Mesh::getIndices() const {
	return _indices;
}