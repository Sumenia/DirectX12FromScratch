#pragma once

# include <d3d12.h>
# include <dxgi1_4.h>

namespace MiniEngine
{
    class D3D12RenderSystem;
    class GraphicPipeline;

    class D3D12VertexBuffer
    {
    public:
        D3D12VertexBuffer(D3D12RenderSystem &system);
        ~D3D12VertexBuffer();
        
        bool                        init(GraphicPipeline &pipeline, unsigned int size, void *vertexData);

        ID3D12Resource              *getBuffer();

    protected:
        D3D12RenderSystem           &_system;
        ID3D12Resource              *_buffer;
        ID3D12Resource              *_bufferUpload;
    };
}