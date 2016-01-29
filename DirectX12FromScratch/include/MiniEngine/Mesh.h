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

	private:
		std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
		std::vector<Vector3f> vertices;
		std::vector<Vector2f> uvs;
		std::vector<Vector3f> normals;
	};
}