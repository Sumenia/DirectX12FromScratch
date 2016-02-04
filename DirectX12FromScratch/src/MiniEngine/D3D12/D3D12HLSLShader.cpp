#include <iostream>
#include <D3Dcompiler.h>
#include "MiniEngine/Helper/d3dx12.h"
#include "MiniEngine/D3D12/D3D12HLSLShader.h"

using namespace MiniEngine;

D3D12HLSLShader::D3D12HLSLShader() : HLSLShader(), _blob(nullptr)
{}

D3D12HLSLShader::~D3D12HLSLShader()
{
    if (_blob)
        _blob->Release();

    _blob = nullptr;
}

void *D3D12HLSLShader::getBytecode()
{
    return (_blob->GetBufferPointer());
}

size_t D3D12HLSLShader::getSize()
{
    return (_blob->GetBufferSize());
}

bool D3D12HLSLShader::compileFromFile(Shader::Type type, std::string const &filename, std::string const &entry)
{
    ID3DBlob        *error = nullptr;
    HRESULT         result;
    std::wstring    wfilename(filename.begin(), filename.end());

    #if defined(_DEBUG)
        UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    #else
        UINT compileFlags = 0;
    #endif

    result = D3DCompileFromFile(wfilename.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry.c_str(), type == VERTEX ? "vs_5_1" : "ps_5_1", compileFlags, 0, &_blob, &error);

    if (FAILED(result))
    {
        if (error)
        {
            std::cout << "Can't compile shader : " << (char*)error->GetBufferPointer() << std::endl;
            error->Release();
        }

        return (false);
    }

    return (true);
}

bool D3D12HLSLShader::compile(Shader::Type type, std::string const &source, std::string const &entry, std::string const &filename)
{
    ID3DBlob        *error = nullptr;
    HRESULT         result;

#if defined(_DEBUG)
    UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    UINT compileFlags = 0;
#endif

    result = D3DCompile(source.c_str(), source.size(), filename.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry.c_str(), type == VERTEX ? "vs_5_1" : "ps_5_1", compileFlags, 0, &_blob, &error);

    if (FAILED(result))
    {
        if (error)
        {
            std::cout << "Can't compile shader : " << (char*)error->GetBufferPointer() << std::endl;
            error->Release();
        }

        return (false);
    }

    return (true);
}

DXGI_FORMAT D3D12HLSLShader::toD3D12(HLSLShader::Input::Format value)
{
    switch (value)
    {
    case HLSLShader::Input::Format::R32G32B32_FLOAT:
        return DXGI_FORMAT_R32G32B32_FLOAT;
    case HLSLShader::Input::Format::R32G32B32A32_FLOAT:
        return DXGI_FORMAT_R32G32B32A32_FLOAT;
    default:
        return DXGI_FORMAT_R32G32B32_FLOAT;
    }
}

D3D12_INPUT_CLASSIFICATION D3D12HLSLShader::toD3D12(HLSLShader::Input::Classification value)
{
    return (static_cast<D3D12_INPUT_CLASSIFICATION>(value));
}