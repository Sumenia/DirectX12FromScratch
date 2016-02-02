#pragma once

# include <string>
# include <algorithm>
# include <vector>
# include <iostream>
# include <fstream>
# include <assimp/scene.h>
# include "Vector.h"

namespace MiniEngine
{
	struct Vertex {
		Vector3f vertice;
		Vector2f uv;
		Vector3f normal;

		bool operator==(const Vertex& toCompare) const;
	};

	class Mesh {
	public:
		Mesh();
		virtual ~Mesh();

		virtual bool loadFromAssimp(aiMesh *mesh);
		const std::vector<unsigned int> &getIndices() const;
		const std::vector<Vertex> &getVertexs() const;

	protected:
		void replaceAll(std::string& str, const std::string& from, const std::string& to);

		std::vector<Vertex>         _vertexs;
		std::vector<unsigned int>   _indices;
	};
}