#include "MiniEngine/Material.h"

using namespace MiniEngine;

unsigned int Material::id_count = 0;

Material::Material() : _id(Material::id_count), _flags(NORMAL_COLOR), _color(1.0f, 1.0f, 1.0f)
{
    Material::id_count++;
}

Material::~Material()
{
	// TO-DO delete Textures for each type.
}

bool	Material::loadFromAssimp(aiMaterial* material)
{
	aiString path;

	std::cout << material->GetTextureCount(aiTextureType_DIFFUSE) << " DIFFUSE" << std::endl;
	std::vector<Texture*> diffuses;
	for (unsigned int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++)
	{
		if (material->GetTexture(aiTextureType_DIFFUSE, i, &path) == AI_SUCCESS)
		{
			Texture *tex = new Texture();
			tex->loadFromFile(path.C_Str());
			std::cout << "Diffuse " << i << " : " << path.C_Str() << std::endl;
			diffuses.push_back(tex);
		}
	}
	if (diffuses.size() != 0)
		_textures.insert(std::pair<TextureType, std::vector<Texture*>>(DIFFUSE, diffuses));

	std::cout << material->GetTextureCount(aiTextureType_SPECULAR) << " SPECULAR" << std::endl;
	for (unsigned int i = 0; i < material->GetTextureCount(aiTextureType_SPECULAR); i++)
	{
		if (material->GetTexture(aiTextureType_SPECULAR, i, &path) == AI_SUCCESS)
			std::cout << "Specular " << i << " : " << path.C_Str() << std::endl;
	}
	std::cout << material->GetTextureCount(aiTextureType_AMBIENT) << " AMBIENT" << std::endl;
	for (unsigned int i = 0; i < material->GetTextureCount(aiTextureType_AMBIENT); i++)
	{
		if (material->GetTexture(aiTextureType_AMBIENT, i, &path) == AI_SUCCESS)
			std::cout << "Ambient " << i << " : " << path.C_Str() << std::endl;
	}
	return false;
}

void Material::useNormalColor()
{
    _flags |= NORMAL_COLOR;
    _flags &= ~TEXTURE;
    _flags &= ~UNIFORM_COLOR;
}

void Material::useUniformColor(Vector3f const &color)
{
    _color = color;

    _flags |= UNIFORM_COLOR;
    _flags &= ~TEXTURE;
    _flags &= ~NORMAL_COLOR;
}

void Material::useTexture(/* TEXTURE */)
{
    // SET TEXTURE

    _flags |= TEXTURE;
    _flags &= ~UNIFORM_COLOR;
    _flags &= ~NORMAL_COLOR;
}

void Material::useNormalScalar()
{
    _flags &= ~NORMAL_MAP;
}

void Material::useNormalMap(/* NORMAL MAP */)
{
    // SET NORMAL MAP

    _flags |= NORMAL_MAP;
}

DWORD64 Material::getFlags() const
{
    return (_flags);
}

unsigned int Material::getId() const
{
    return (_id);
}

std::string Material::generateHLSLShader(Shader::Type type)
{
    std::stringstream   source;

    setFlagToShader(source, "NORMAL_COLOR", NORMAL_COLOR);
    setFlagToShader(source, "UNIFORM_COLOR", UNIFORM_COLOR);
    setFlagToShader(source, "TEXTURE", TEXTURE);
    setFlagToShader(source, "NORMAL_MAP", NORMAL_MAP);

    if (type == Shader::VERTEX)
        source << "#include \"vs.hlsl\"" << std::endl;
    else if (type == Shader::PIXEL)
        source << "#include \"ps.hlsl\"" << std::endl;

    return (source.str());
}

void Material::setFlagToShader(std::stringstream &source, std::string const &define, Flag flag)
{
    source << "#define " << define << " " << (_flags & flag) << std::endl;
}