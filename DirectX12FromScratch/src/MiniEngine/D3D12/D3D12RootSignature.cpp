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

bool D3D12RootSignature::init(D3D12RenderSystem &system)
{
    HRESULT                     result;
    ID3DBlob                    *signature = nullptr;
    ID3DBlob                    *error = nullptr;

    CD3DX12_DESCRIPTOR_RANGE    ranges[2];
    CD3DX12_ROOT_PARAMETER      parameters[2];

    ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
    ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);

    parameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_ALL);
    parameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_ALL);

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Init(2, parameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

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