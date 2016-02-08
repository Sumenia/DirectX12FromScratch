#pragma once

# include <DirectXMath.h>
# include "MiniEngine/RenderableModel.h"
# include "MiniEngine/D3D12/D3D12VertexBuffer.h"
# include "MiniEngine/D3D12/D3D12IndexBuffer.h"
# include "MiniEngine/D3D12/D3D12GraphicPipeline.h"

namespace MiniEngine
{
	struct D3D12Vertex
	{
		DirectX::XMFLOAT3 vertice;
		//DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT3 normal;
	};

	class D3D12RenderableModel : public RenderableModel
	{
	public:
		D3D12RenderableModel(RenderSystem& system);
		~D3D12RenderableModel();

		virtual bool        loadFromFile(D3D12RenderSystem &system, const std::string &file);

		bool                initVertexBuffer(D3D12RenderSystem &system);
		bool                initIndexBuffer(D3D12RenderSystem &system);

		bool                render(Camera &camera, CommandList &commandList);

	protected:
		D3D12VertexBuffer   *_vertexBuffer;
		D3D12IndexBuffer    *_indexBuffer;

		D3D12Vertex         *_vertexData;
		unsigned int        *_indexData;
	};
}