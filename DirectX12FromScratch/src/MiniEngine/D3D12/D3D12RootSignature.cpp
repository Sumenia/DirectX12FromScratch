#include <iostream>
#include "MiniEngine/D3D12/D3D12RootSignature.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"
#include "MiniEngine/D3D12/D3D12Device.h"

using namespace MiniEngine;

D3D12RootSignature::D3D12RootSignature() : _signature(nullptr)
{}

D3D12RootSignature::~D3D12RootSignature()
{
    if (_signature)
        _signature->Release();
    _signature = nullptr;
}

bool D3D12RootSignature::init(D3D12RenderSystem &system, bool ambient, bool diffuse, bool specular)
{
    HRESULT                     result;
    ID3DBlob                    *signature = nullptr;
    ID3DBlob                    *error = nullptr;

    unsigned int                nbSrvs = (int)ambient + (int)diffuse + (int)specular;
    unsigned int                cursor = 0;
    CD3DX12_DESCRIPTOR_RANGE	*ranges = nullptr;

    if (nbSrvs > 0)
    {
        ranges = new CD3DX12_DESCRIPTOR_RANGE[nbSrvs];

        if (ambient)
        {
            ranges[cursor].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
            cursor++;
        }

        if (diffuse)
        {
            ranges[cursor].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);
            cursor++;
        }

        if (specular)
        {
            ranges[cursor].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);
            cursor++;
        }
    }

    CD3DX12_ROOT_PARAMETER      parameters[5];
    
    parameters[0].InitAsConstantBufferView(0);
    parameters[1].InitAsConstantBufferView(1);
    parameters[2].InitAsConstantBufferView(2);
    parameters[3].InitAsConstantBufferView(3);

    if (nbSrvs > 0)
	    parameters[4].InitAsDescriptorTable(nbSrvs, ranges);

	D3D12_STATIC_SAMPLER_DESC sampler = {};
	sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.MipLODBias = 0;
	sampler.MaxAnisotropy = 0;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler.MinLOD = 0.0f;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	sampler.ShaderRegister = 0;
	sampler.RegisterSpace = 0;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Init(nbSrvs > 0 ? 5 : 4, parameters, 1, &sampler, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);

    if (FAILED(result))
    {
        std::cout << "Can't serialize D3D12 Root Signature" << std::endl;
        return (false);
    }

    result = system.getDevice()->getNative()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)&_signature);

    if (FAILED(result))
    {
        delete signature;
        delete error;

        std::cout << "Can't create D3D12 Root Signature" << std::endl;
        return (false);
    }

    return (true);
}

ID3D12RootSignature *D3D12RootSignature::getNative()
{
    return (_signature);
}