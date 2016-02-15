#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "MiniEngine/Model.h"

using namespace MiniEngine;

Model::Model(RenderSystem& system) : _system(system), _isLoaded(false)
{}

Model::~Model()
{
	/*while (_materials.size() != 0)
	{
		delete (_materials.front());
		_materials.pop_front();
	}*/
}

bool			Model::isLoaded() const
{
	return _isLoaded;
}

bool			Model::loadFromFile(const std::string &file)
{
	Assimp::Importer		importer;
	std::vector<Material*>	materials;

	_path = file.substr(0, file.find_last_of('/'));
	_file = file.substr(file.find_last_of('/') + 1);
	const aiScene* scene = importer.ReadFile(file,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_FixInfacingNormals |
		aiProcess_GenUVCoords |
		aiProcess_FlipWindingOrder |
        aiProcess_ValidateDataStructure |
        aiProcess_ImproveCacheLocality |
        aiProcess_RemoveRedundantMaterials |
        aiProcess_FindDegenerates |
        aiProcess_FindInvalidData |
        aiProcess_OptimizeMeshes |
		aiProcess_GenSmoothNormals |
        aiProcess_PreTransformVertices |
        aiProcess_TransformUVCoords |
        aiProcess_FlipUVs
    );

	if (!scene)
	{
		std::cerr << importer.GetErrorString() << std::endl;
		return false;
	}

	materials.resize(scene->mNumMaterials);
	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		Material *material = _system.createMaterial();

		if (!material->loadFromAssimp(scene->mMaterials[i], _path) || !material->finalize())
		{
			delete material;
			return (false);
		}
		_system.registerMaterial(material);
		materials[i] = material;
	}

	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

		mesh->loadFromAssimp(scene->mMeshes[i], materials);
		_meshs.push_back(mesh);
	}

	_isLoaded = true;

	return true;
}

unsigned int		Model::getVertexsSize() const
{
	unsigned int	size;

	size = 0;

	for (auto &&mesh : _meshs)
	{
		size += (unsigned int) mesh->vertexs.size();
	}

	return size;
}

unsigned int		Model::getIndicesSize() const
{
	unsigned int	size;

	size = 0;

	for (auto &&mesh : _meshs)
	{
		size += (unsigned int) mesh->indices.size();
	}

	return size;
}

const std::list<std::shared_ptr<Mesh> >		&Model::getMeshs()
{
	return _meshs;
}

const std::string&			Model::getPath() const
{
	return (_path);
}