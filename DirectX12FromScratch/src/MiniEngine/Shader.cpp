#include "MiniEngine/Shader.h"

using namespace MiniEngine;

Shader::Shader(std::string const &filename, std::string const &entry) : _filename(filename), _entry(entry)
{}

Shader::~Shader()
{}