#pragma once

# include <string>
# include <vector>
#include <iostream>
#include <fstream>
# include "Vector.h"

namespace MiniEngine
{
	class Mesh {
	public:
		Mesh();
		~Mesh();

		bool loadObjFromFile(const std::string &file);

		const std::vector<unsigned int> &getVertexIndices() const;
		const std::vector<unsigned int> &getUvIndices() const;
		const std::vector<unsigned int> &getNormalIndices() const;

		const std::vector<Vector3f> &getVertexs() const;
		const std::vector<Vector2f> &getUvs() const;
		const std::vector<Vector3f> &getNormals() const;

		bool isLoaded() const;

	private:
		std::vector<unsigned int> _vertexIndices;
		std::vector<unsigned int> _uvIndices;
		std::vector<unsigned int> _normalIndices;

		std::vector<Vector3f> _vertexs;
		std::vector<Vector2f> _uvs;
		std::vector<Vector3f> _normals;

		bool _isLoaded;
	};
}