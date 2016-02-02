# include <assimp/Importer.hpp>
# include <assimp/scene.h>
# include <assimp/postprocess.h>
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

	//std::ifstream file(path);
	Assimp::Importer importer;

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll 
	// propably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	// If the import failed, report it
	if (!scene)
	{
		std::cerr << importer.GetErrorString() << std::endl;
		return false;
	}

	_vertexs.clear();
	_indices.clear();
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		auto &&mesh = scene->mMeshes[i];

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
			for (int k = 2; k >= 0; k--)
			{
				_indices.push_back(face.mIndices[k]);
			}
		}
	}

	std::cout << "LOADED" << std::endl;
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