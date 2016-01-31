#include "MiniEngine/D3D12/D3D12RenderableMesh.h"
#include "MiniEngine/D3D12/D3D12CommandList.h"

using namespace MiniEngine;

D3D12RenderableMesh::D3D12RenderableMesh() : _vertexBuffer(nullptr), _indexBuffer(nullptr)
{}

D3D12RenderableMesh::~D3D12RenderableMesh()
{
    delete _vertexData;
    _vertexData = nullptr;

    delete _indexData;
    _indexData = nullptr;

    delete _vertexBuffer;
    _vertexBuffer = nullptr;

    delete _indexBuffer;
    _indexBuffer = nullptr;
}

bool D3D12RenderableMesh::loadObjFromFile(D3D12RenderSystem &system, D3D12GraphicPipeline &pipeline, const std::string &file)
{
    if (!Mesh::loadObjFromFile(file))
        return (false);

    return (initVertexBuffer(system, pipeline) && initIndexBuffer(system, pipeline));
}

bool D3D12RenderableMesh::initVertexBuffer(D3D12RenderSystem &system, D3D12GraphicPipeline &pipeline)
{
    _vertexData = new D3D12Vertex[_vertexs.size()];

    for (unsigned int i = 0; i < _vertexs.size(); i++)
    {
        std::cout << _vertexs[i].vertice.x << " / " << _vertexs[i].vertice.y << " / " << _vertexs[i].vertice.z << std::endl;
        _vertexData[i].vertice = { _vertexs[i].vertice.x, _vertexs[i].vertice.y, _vertexs[i].vertice.z };
        //_vertexData[i].normal = { _vertexs[i].normal.x, _vertexs[i].normal.y, _vertexs[i].normal.z };
        //_vertexData[i].uv = { _vertexs[i].uv.x, _vertexs[i].uv.y };
    }

    _vertexBuffer = new D3D12VertexBuffer(system);
    return (_vertexBuffer->init(pipeline, sizeof(_vertexData), _vertexData));
}

bool D3D12RenderableMesh::initIndexBuffer(D3D12RenderSystem &system, D3D12GraphicPipeline &pipeline)
{
    _indexData = new unsigned short[_indices.size()];

    for (unsigned int i = 0; i < _indices.size(); i++)
    {
        std::cout << _indices[i] << ", ";
        if (i % 3 == 2)
            std::cout << std::endl;

        _indexData[i] = _indices[i];
    }

    _indexBuffer = new D3D12IndexBuffer(system);
    return (_indexBuffer->init(pipeline, sizeof(_indexData), _indexData));
}

bool D3D12RenderableMesh::render(Camera &camera, CommandList &commandList)
{
    D3D12_VERTEX_BUFFER_VIEW    vertexView;
    D3D12_INDEX_BUFFER_VIEW     indexView;

    D3D12CommandList            &d3d12CommandList = dynamic_cast<D3D12CommandList&>(commandList);

    vertexView.BufferLocation = _vertexBuffer->getBuffer()->GetGPUVirtualAddress();
    vertexView.StrideInBytes = sizeof(D3D12Vertex);
    vertexView.SizeInBytes = sizeof(_vertexData);

    indexView.BufferLocation = _indexBuffer->getBuffer()->GetGPUVirtualAddress();
    indexView.SizeInBytes = sizeof(_indexData);
    indexView.Format = DXGI_FORMAT_R16_UINT;

    d3d12CommandList.getNative()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    d3d12CommandList.getNative()->IASetVertexBuffers(0, 1, &vertexView);
    d3d12CommandList.getNative()->IASetIndexBuffer(&indexView);
    d3d12CommandList.getNative()->DrawIndexedInstanced(_indices.size(), 1, 0, 0, 0);

    std::cout << "RENDER " << _indices.size() << " indices" << std::endl;

    return (true);
}