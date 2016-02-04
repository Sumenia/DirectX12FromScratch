#pragma once

# include <d3d12.h>
# include <dxgi1_4.h>
# include "MiniEngine/HLSLShader.h"

namespace MiniEngine
{
    class D3D12HLSLShader : public HLSLShader
    {
    public:
        D3D12HLSLShader();
        ~D3D12HLSLShader();

        void            *getBytecode();
        size_t          getSize();

        bool            compileFromFile(Shader::Type type, std::string const &filename, std::string const &entry);
        bool            compile(Shader::Type type, std::string const &source, std::string const &entry, std::string const &filename = "./Assets/shaders/main.hlsl");

        static DXGI_FORMAT                  toD3D12(HLSLShader::Input::Format value);
        static D3D12_INPUT_CLASSIFICATION   toD3D12(HLSLShader::Input::Classification value);
        
    protected:
        ID3DBlob        *_blob;
    };
}