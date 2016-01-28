#pragma once

# include "MiniEngine/Shader.h"

namespace MiniEngine
{
    class HLSLShader : public Shader
    {
    public:
        HLSLShader(std::string const &filename, std::string const &entry);
        virtual ~HLSLShader();
    };
}