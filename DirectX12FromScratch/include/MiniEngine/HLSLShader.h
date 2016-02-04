#pragma once

# include "MiniEngine/Shader.h"

namespace MiniEngine
{
    class HLSLShader : public Shader
    {
    public:
        struct Input
        {
            enum Format
            {
                R32G32B32_FLOAT,
                R32G32B32A32_FLOAT
            };

            enum Classification
            {
                PER_VERTEX = 0,
                PER_INSTANCE = 1
            };

            const char              *semanticName;
            unsigned int            semanticIndex;
            Format                  format;
            unsigned int            inputSlot;
            unsigned int            alignedByteOffset;
            Classification          inputSlotClass;
            unsigned int            instanceDataStepRate;
        };

    public:
        HLSLShader();
        virtual ~HLSLShader();

        virtual bool                compile(Shader::Type type, std::string const &source, std::string const &entry, std::string const &filename = "./Assets/shaders/main.hlsl") = 0;

    };
}