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
        HLSLShader(std::string const &filename, std::string const &entry);
        virtual ~HLSLShader();
    };
}