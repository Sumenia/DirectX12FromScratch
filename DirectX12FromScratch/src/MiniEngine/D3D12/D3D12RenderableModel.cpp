#include "MiniEngine/D3D12/D3D12RenderableModel.h"
#include "MiniEngine/D3D12/D3D12CommandList.h"

using namespace MiniEngine;

D3D12RenderableModel::D3D12RenderableModel() : _vertexBuffer(nullptr), _indexBuffer(nullptr)
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

bool D3D12RenderableModel::loadObjFromFile(D3D12RenderSystem &system, D3D12GraphicPipeline &pipeline, const std::string &file)
{
	if (!Model::loadObjFromFile(file))
		return (false);

	return (initVertexBuffer(system, pipeline) && initIndexBuffer(system, pipeline));
}

bool D3D12RenderableModel::initVertexBuffer(D3D12RenderSystem &system, D3D12GraphicPipeline &pipeline)
{
	unsigned int size = getVertexsSize();
	_vertexData = new D3D12Vertex[size];

	for (auto &&mesh : getMeshs())
	{
		auto &&vertexs = mesh->getVertexs();
		for (unsigned int i = 0; i < vertexs.size(); i++)
		{
			_vertexData[i].vertice = { vertexs[i].vertice.x, vertexs[i].vertice.y, vertexs[i].vertice.z };
			_vertexData[i].normal = { vertexs[i].normal.x, vertexs[i].normal.y, vertexs[i].normal.z };
			//_vertexData[i].uv = { _vertexs[i].uv.x, _vertexs[i].uv.y };
		}
	}
	

	_vertexBuffer = new D3D12VertexBuffer(system);
	return (_vertexBuffer->init(pipeline, sizeof(D3D12Vertex) * size, _vertexData));
}

bool D3D12RenderableModel::initIndexBuffer(D3D12RenderSystem &system, D3D12GraphicPipeline &pipeline)
{
	unsigned int size = getIndicesSize();
	unsigned int indexOffset = 0;
	_indexData = new unsigned short[size];

	for (auto &&mesh : getMeshs())
	{
		auto &&indices = mesh->getIndices();
		for (unsigned int i = 0; i < indices.size(); i++)
		{
			_indexData[i + indexOffset] = indices[i] + indexOffset;
		}
		indexOffset += indices.size();
	}

	_indexBuffer = new D3D12IndexBuffer(system);
	return (_indexBuffer->init(pipeline, sizeof(unsigned short) * size, _indexData));
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
	indexView.SizeInBytes = sizeof(unsigned short) * getIndicesSize();
	indexView.Format = DXGI_FORMAT_R16_UINT;

	d3d12CommandList.getNative()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3d12CommandList.getNative()->IASetVertexBuffers(0, 1, &vertexView);
	d3d12CommandList.getNative()->IASetIndexBuffer(&indexView);
	d3d12CommandList.getNative()->DrawIndexedInstanced(getIndicesSize(), 1, 0, 0, 0);

	return (true);
}