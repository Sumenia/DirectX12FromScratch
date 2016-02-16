#include <d3d12.h>
#include "MiniEngine/Material.h"
#include "MiniEngine/RenderSystem.h"

using namespace MiniEngine;

unsigned int Material::id_count = 0;

Material::Material(RenderSystem &system) : _system(system), _id(Material::id_count), _flags(0), _ka(1.0f, 1.0f, 1.0f), _kd(1.0f, 1.0f, 1.0f), _ks(1.0f, 1.0f, 1.0f), _shininess(32)
{
    Material::id_count++;

    _textures[AMBIENT] = nullptr;
    _textures[DIFFUSE] = nullptr;
    _textures[SPECULAR] = nullptr;
    _textures[NORMAL] = nullptr;
}

Material::~Material()
{
	for (auto &&pair : _textures)
	{
		delete pair.second;
		pair.second = nullptr;
	}
}

bool	Material::loadFromAssimp(aiMaterial* material, const std::string& path)
{
	aiString file;

    if (material->GetTextureCount(aiTextureType_AMBIENT) > 0 &&
        material->GetTexture(aiTextureType_AMBIENT, 0, &file) == AI_SUCCESS)
    {
        Texture *tex = _system.createTexture(path + "/" + file.C_Str());

        if (!tex)
            return (false);

        std::cout << "Ambient : " << file.C_Str() << std::endl;
        useTexture(AMBIENT, tex);
    }

	if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0 &&
		material->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
	{
		Texture *tex = _system.createTexture(path + "/" + file.C_Str());

        if (!tex)
            return (false);

		std::cout << "Diffuse : " << path + "/" + file.C_Str() << std::endl;
		useTexture(DIFFUSE, tex);
	}

	if (material->GetTextureCount(aiTextureType_SPECULAR) > 0 &&
		material->GetTexture(aiTextureType_SPECULAR, 0, &file) == AI_SUCCESS)
	{
        Texture *tex = _system.createTexture(path + "/" + file.C_Str());

        if (!tex)
            return (false);

        std::cout << "Specular : " << file.C_Str() << std::endl;
		useTexture(SPECULAR, tex);
	}

	Texture *tex = _system.createNormalMap("./Assets/models/normal_map.png");

	if (!tex)
		return (false);

	useNormalMap(tex);

	return true;
}

void Material::useNormalColor()
{
    _flags |= NORMAL_COLOR;
    _flags &= ~TEXTURE_DIFFUSE;
}

void Material::useDiffuseColor(Vector3f const &color)
{
    _kd = color;
    
    _flags &= ~TEXTURE_DIFFUSE;
    _flags &= ~NORMAL_COLOR;

    delete _textures[DIFFUSE];
    _textures[DIFFUSE] = nullptr;
}

void Material::useAmbientColor(Vector3f const &color)
{
    _ka = color;
    _flags &= ~TEXTURE_AMBIENT;

    delete _textures[AMBIENT];
    _textures[AMBIENT] = nullptr;
}

void Material::useSpecularColor(Vector3f const &color) {
    _ks = color;
    _flags &= ~TEXTURE_SPECULAR;

    delete _textures[SPECULAR];
    _textures[SPECULAR] = nullptr;
}

void Material::setShininess(float shininess)
{
    _shininess = shininess;
}

void Material::useTexture(TextureType t, Texture *tex)
{
    if (t == DIFFUSE)
    {
        _flags |= TEXTURE_DIFFUSE;
        _flags &= ~NORMAL_COLOR;
    }
    else if (t == AMBIENT)
        _flags |= TEXTURE_AMBIENT;
    else if (t == SPECULAR)
        _flags |= TEXTURE_SPECULAR;

    _textures[t] = tex;
}

void Material::useNormalScalar()
{
    _flags &= ~NORMAL_MAP;

    delete _textures[NORMAL];
    _textures[NORMAL] = nullptr;
}

void Material::useNormalMap(Texture *tex)
{
    _flags |= NORMAL_MAP;

    _textures[NORMAL] = tex;
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
    setFlagToShader(source, "TEXTURE_AMBIENT", TEXTURE_AMBIENT);
    setFlagToShader(source, "TEXTURE_DIFFUSE", TEXTURE_DIFFUSE);
    setFlagToShader(source, "TEXTURE_SPECULAR", TEXTURE_SPECULAR);
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

bool Material::haveAmbientMap() const
{
    return (!!_textures.at(AMBIENT));
}

bool Material::haveDiffuseMap() const
{
    return (!!_textures.at(DIFFUSE));
}

bool Material::haveSpecularMap() const
{
    return (!!_textures.at(SPECULAR));
}

bool Material::haveNormalMap() const
{
    return (!!_textures.at(NORMAL));
}