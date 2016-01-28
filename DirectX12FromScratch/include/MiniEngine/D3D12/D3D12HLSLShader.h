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

        static DXGI_FORMAT                  toD3D12(HLSLShader::Input::Format value);
        static D3D12_INPUT_CLASSIFICATION   toD3D12(HLSLShader::Input::Classification value);
        
    protected:
        ID3DBlob        *_blob;
    };
}