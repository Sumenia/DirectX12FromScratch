#include <iostream>
#include "MiniEngine/Helper/d3dx12.h"
#include "MiniEngine/D3D12/D3D12GraphicPipeline.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"
#include "MiniEngine/D3D12/D3D12Device.h"

using namespace MiniEngine;

D3D12GraphicPipeline::D3D12GraphicPipeline(D3D12RenderSystem &system) : GraphicPipeline(system), _system(system), _pipeline(nullptr)
{
    ZeroMemory(&_desc, sizeof(_desc));

    _desc.NodeMask = 1;
    _desc.SampleMask = UINT_MAX;
    _desc.SampleDesc.Count = 1;
    _desc.InputLayout.NumElements = 0;
    _desc.pRootSignature = _system.getRootSignature()->getNative();
    _desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    _desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    _desc.DepthStencilState.DepthEnable = FALSE;
    _desc.DepthStencilState.StencilEnable = FALSE;
    _desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    _desc.NumRenderTargets = 1;
    _desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
}

D3D12GraphicPipeline::~D3D12GraphicPipeline()
{
    if (_pipeline)
        _pipeline->Release();

    _pipeline = nullptr;
}

void D3D12GraphicPipeline::addVertexShader(Shader &shader)
{
    _desc.VS = CD3D12_SHADER_BYTECODE(shader.getBytecode(), shader.getSize());
}

void D3D12GraphicPipeline::addPixelShader(Shader &shader)
{
    _desc.PS = CD3D12_SHADER_BYTECODE(shader.getBytecode(), shader.getSize());
}

bool D3D12GraphicPipeline::finalize()
{
    HRESULT result;
    
    result = _system.getDevice()->getNative()->CreateGraphicsPipelineState(&_desc, __uuidof(ID3D12PipelineState), (void**)&_pipeline);

    if (FAILED(result))
    {
        std::cout << "Can't create Graphics Pipeline" << std::endl;
        return (false);
    }

    return (true);
}