#include "MiniEngine/D3D12/D3D12RenderableModel.h"
#include "MiniEngine/D3D12/D3D12CommandList.h"

using namespace MiniEngine;

D3D12RenderableModel::D3D12RenderableModel(RenderSystem& system) : RenderableModel(system), _vertexBuffer(nullptr), _indexBuffer(nullptr)
{}

D3D12RenderableModel::~D3D12RenderableModel()
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

bool D3D12RenderableModel::loadFromFile(D3D12RenderSystem &system, const std::string &file)
{
	if (!Model::loadFromFile(file))
		return (false);

	return (initVertexBuffer(system) && initIndexBuffer(system));
}

bool D3D12RenderableModel::initVertexBuffer(D3D12RenderSystem &system)
{
    unsigned int    i = 0;
	unsigned int    size = getVertexsSize();

	_vertexData = new D3D12Vertex[size];

	for (auto &&mesh : getMeshs())
	{
		auto &&vertexs = mesh->vertexs;

        mesh->offset = i;

		for (unsigned int k = 0; k < vertexs.size(); k++, i++)
		{
			_vertexData[i].vertice = { vertexs[k].vertice.x, vertexs[k].vertice.y, vertexs[k].vertice.z };
			_vertexData[i].normal = { vertexs[k].normal.x, vertexs[k].normal.y, vertexs[k].normal.z };
			//_vertexData[i].uv = { _vertexs[i].uv.x, _vertexs[i].uv.y };
		}
	}
	
	_vertexBuffer = new D3D12VertexBuffer(system);
	return (_vertexBuffer->init(sizeof(D3D12Vertex) * size, _vertexData));
}

bool D3D12RenderableModel::initIndexBuffer(D3D12RenderSystem &system)
{
    unsigned int    i = 0;
	unsigned int    size = getIndicesSize();

	_indexData = new unsigned int[size];

	for (auto &&mesh : getMeshs())
	{
		auto &&indices = mesh->indices;

        mesh->idxOffset = i;

		for (unsigned int k = 0; k < indices.size(); k++, i++)
			_indexData[i] = indices[k] + mesh->offset;
	}

	_indexBuffer = new D3D12IndexBuffer(system);
	return (_indexBuffer->init(sizeof(unsigned int) * size, _indexData));
}

bool D3D12RenderableModel::render(Camera &camera, CommandList &commandList)
{
	D3D12_VERTEX_BUFFER_VIEW    vertexView;
	D3D12_INDEX_BUFFER_VIEW     indexView;

	D3D12CommandList            &d3d12CommandList = dynamic_cast<D3D12CommandList&>(commandList);

	vertexView.BufferLocation = _vertexBuffer->getBuffer()->GetGPUVirtualAddress();
	vertexView.StrideInBytes = sizeof(D3D12Vertex);
	vertexView.SizeInBytes = sizeof(D3D12Vertex) * getVertexsSize();

	indexView.BufferLocation = _indexBuffer->getBuffer()->GetGPUVirtualAddress();
	indexView.SizeInBytes = sizeof(unsigned int) * getIndicesSize();
	indexView.Format = DXGI_FORMAT_R32_UINT;

    d3d12CommandList.getNative()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    d3d12CommandList.getNative()->IASetVertexBuffers(0, 1, &vertexView);
    d3d12CommandList.getNative()->IASetIndexBuffer(&indexView);

    for (auto &&mesh : getMeshs())
    {
		OutputDebugString("test1\n");
        if (!commandList.bindMaterial(mesh->materialId))
            return (false);
		OutputDebugString("test2\n");

        d3d12CommandList.getNative()->DrawIndexedInstanced(mesh->indices.size(), 1, mesh->idxOffset, 0, 0);
    }
	return (true);
}