#include <iostream>
#include "MiniEngine/Helper/d3dx12.h"
#include "MiniEngine/D3D12/D3D12GraphicPipeline.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"
#include "MiniEngine/D3D12/D3D12Device.h"

using namespace MiniEngine;

D3D12GraphicPipeline::D3D12GraphicPipeline(D3D12RenderSystem &system) : GraphicPipeline(system), _system(system), _pipeline(nullptr), _rootSignature(nullptr)
{
    ZeroMemory(&_desc, sizeof(_desc));

    _desc.NodeMask = 1;
    _desc.SampleMask = UINT_MAX;
    _desc.SampleDesc.Count = 1;
    _desc.InputLayout.NumElements = 0;
    _desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    _desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    _desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    _desc.NumRenderTargets = 1;
    _desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    _desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
}

D3D12GraphicPipeline::~D3D12GraphicPipeline()
{
    if (_pipeline)
        _pipeline->Release();

    _pipeline = nullptr;

    delete _rootSignature;
    _rootSignature = nullptr;
}

void D3D12GraphicPipeline::setRootSignature(D3D12RootSignature *rootSignature)
{
    _rootSignature = rootSignature;
}

D3D12RootSignature *D3D12GraphicPipeline::getRootSignature()
{
    return (_rootSignature);
}

void D3D12GraphicPipeline::setInputs(unsigned int nb, const void *inputs)
{
    setInputs(nb, static_cast<const HLSLShader::Input*>(inputs));
}

void D3D12GraphicPipeline::setInputs(unsigned int nb, const HLSLShader::Input *inputs)
{
    _desc.InputLayout.NumElements = nb;

    if (nb > 0)
    {
        D3D12_INPUT_ELEMENT_DESC *newInputs = new D3D12_INPUT_ELEMENT_DESC[nb];

        for (unsigned int i = 0; i < nb; i++)
        {
            newInputs[i] = {
                inputs[i].semanticName,
                inputs[i].semanticIndex,
                D3D12HLSLShader::toD3D12(inputs[i].format),
                inputs[i].inputSlot,
                inputs[i].alignedByteOffset,
                D3D12HLSLShader::toD3D12(inputs[i].inputSlotClass),
                inputs[i].instanceDataStepRate
            };
        }
        
        _inputs.reset(newInputs);
    }
    else
        _inputs = nullptr;
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

    if (!_rootSignature)
        return (false);

    _desc.pRootSignature = _rootSignature->getNative();
    _desc.InputLayout.pInputElementDescs = _inputs.get();
    
    result = _system.getDevice()->getNative()->CreateGraphicsPipelineState(&_desc, __uuidof(ID3D12PipelineState), (void**)&_pipeline);

    if (FAILED(result))
    {
        std::cout << "Can't create Graphics Pipeline" << std::endl;
        return (false);
    }

    return (true);
}

ID3D12PipelineState *D3D12GraphicPipeline::getNative()
{
    return (_pipeline);
}