#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "MiniEngine/Model.h"

using namespace MiniEngine;

Model::Model(): _isLoaded(false) {}

Model::~Model() {}

bool			Model::isLoaded() const
{
	return _isLoaded;
}

bool			Model::loadObjFromFile(const std::string &file)
{
	//std::ifstream file(path);
	Assimp::Importer importer;

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll 
	// propably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(file,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_FixInfacingNormals |
		aiProcess_GenUVCoords |
		aiProcess_FlipWindingOrder |
		aiProcess_GenNormals);

	// If the import failed, report it
	if (!scene)
	{
		std::cerr << importer.GetErrorString() << std::endl;
		return false;
	}


	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

		mesh->loadFromAssimp(scene->mMeshes[i]);
		_meshs.push_back(mesh);
	}

	/*for (int j = 0; j < scene->mNumMaterials; j++)
	{
	auto &&material = scene->mMaterials[j];
	int texIndex = 0;
	aiString path;

	if (material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path) == AI_SUCCESS)
	{
	std::cout << "TEXTURE PATH: " << path.C_Str() << std::endl;
	}
	else
	std::cout << "NO DIFFUSE TEXTURE" << std::endl;
	}*/

	_isLoaded = true;
	std::cout << "LOADED" << std::endl;

	return true;
}

unsigned int		Model::getVertexsSize() const
{
	unsigned int	size;

	size = 0;
	for (auto &&mesh : _meshs)
	{
		size += mesh->getVertexs().size();
	}

	return size;
}

unsigned int		Model::getIndicesSize() const
{
	unsigned int	size;

	size = 0;
	for (auto &&mesh : _meshs)
	{
		size += mesh->getIndices().size();
	}

	return size;
}

const std::list<std::shared_ptr<Mesh> >		&Model::getMeshs()
{
	return _meshs;
}