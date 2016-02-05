#pragma once

# include <d3d12.h>
# include <dxgi1_4.h>
# include <memory>
# include "MiniEngine/GraphicPipeline.h"
# include "MiniEngine/D3D12/D3D12Fence.h"
# include "MiniEngine/HLSLShader.h"

struct CD3D12_SHADER_BYTECODE : public D3D12_SHADER_BYTECODE
{
    CD3D12_SHADER_BYTECODE()
    {}
    explicit CD3D12_SHADER_BYTECODE(const D3D12_SHADER_BYTECODE& o) :
        D3D12_SHADER_BYTECODE(o)
    {}
    explicit CD3D12_SHADER_BYTECODE(const void* _pShaderBytecode, SIZE_T _BytecodeLength)
    {
        pShaderBytecode = _pShaderBytecode;
        BytecodeLength = _BytecodeLength;
    }
    ~CD3D12_SHADER_BYTECODE() {}
    operator const D3D12_SHADER_BYTECODE&() const { return *this; }
};

namespace MiniEngine
{
    class D3D12RootSignature;

    class D3D12GraphicPipeline : public GraphicPipeline
    {
    public:
        D3D12GraphicPipeline(D3D12RenderSystem &system);
        ~D3D12GraphicPipeline();

        void                                    setRootSignature(D3D12RootSignature *rootSignature);
        D3D12RootSignature                      *getRootSignature();

        void                                    setInputs(unsigned int nb, const void *inputs);
        void                                    setInputs(unsigned int nb, const HLSLShader::Input *inputs);

        void                                    addVertexShader(Shader &shader);
        void                                    addPixelShader(Shader &shader);

        bool                                    finalize();

        ID3D12PipelineState                     *getNative();

    protected:
        D3D12RenderSystem                       &_system;
        D3D12_GRAPHICS_PIPELINE_STATE_DESC      _desc;
        ID3D12PipelineState                     *_pipeline;
        D3D12RootSignature                      *_rootSignature;
        std::shared_ptr<const D3D12_INPUT_ELEMENT_DESC>    _inputs;
    };
}