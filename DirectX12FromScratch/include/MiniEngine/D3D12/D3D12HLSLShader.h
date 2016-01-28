#pragma once

# include <d3d12.h>
# include <dxgi1_4.h>
# include "MiniEngine/HLSLShader.h"

namespace MiniEngine
{
    class D3D12HLSLShader : public HLSLShader
    {
    public:
        D3D12HLSLShader(std::string const &filename, std::string const &entry);
        ~D3D12HLSLShader();

        void            *getBytecode();
        size_t          getSize();

        bool            compile(Shader::Type type);
        
    protected:
        ID3DBlob        *_blob;
    };
}