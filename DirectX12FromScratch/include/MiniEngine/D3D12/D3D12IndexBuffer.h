#pragma once

# include <d3d12.h>
# include <dxgi1_4.h>

namespace MiniEngine
{
    class D3D12RenderSystem;

    class D3D12IndexBuffer
    {
    public:
        D3D12IndexBuffer(D3D12RenderSystem &system);
        ~D3D12IndexBuffer();

        bool                        init(GraphicPipeline &pipeline, unsigned int size, void *vertexData);

        ID3D12Resource              *getBuffer();
        D3D12_INDEX_BUFFER_VIEW     *getView();

    protected:
        D3D12RenderSystem           &_system;
        ID3D12Resource              *_buffer;
        ID3D12Resource              *_bufferUpload;
        D3D12_INDEX_BUFFER_VIEW     _view;
    };
}
