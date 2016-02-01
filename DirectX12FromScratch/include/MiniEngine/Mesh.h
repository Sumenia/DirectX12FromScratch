#pragma once

# include <string>
# include <algorithm>
# include <vector>
# include <iostream>
# include <fstream>
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

		virtual bool loadObjFromFile(const std::string &file);
		const std::vector<unsigned int> &getIndices() const;
		const std::vector<Vertex> &getVertexs() const;
		bool isLoaded() const;

	protected:
		void replaceAll(std::string& str, const std::string& from, const std::string& to);

		std::vector<int>            _verticesIndices;
		std::vector<int>            _uvIndices;
		std::vector<int>            _normalIndices;

		std::vector<Vector3f>       _vertices;
		std::vector<Vector2f>       _uvs;
		std::vector<Vector3f>       _normals;

		std::vector<Vertex>         _vertexs;
		std::vector<unsigned int>   _indices;

		bool _isLoaded;
	};
}