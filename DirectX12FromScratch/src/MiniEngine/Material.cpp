#include "MiniEngine/Material.h"

using namespace MiniEngine;

unsigned int Material::id_count = 0;

Material::Material() : _id(Material::id_count), _flags(NORMAL_COLOR), _color(1.0f, 1.0f, 1.0f)
{
    Material::id_count++;
}

Material::~Material()
{}

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