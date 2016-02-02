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

Mesh::Mesh() : offset(0)
{}

Mesh::~Mesh()
{}

bool Mesh::loadFromAssimp(aiMesh *mesh) {
	for (unsigned int j = 0; j < mesh->mNumVertices; j++)
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

		vertexs.push_back(vertex);
	}

	for (unsigned int j = 0; j < mesh->mNumFaces; j++)
	{
		auto &&face = mesh->mFaces[j];
		for (int k = 0; k < 3; k++)
		{
			indices.push_back(face.mIndices[k]);
		}
	}

	return true;
}