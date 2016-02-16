#pragma once

# include <string>
# include <algorithm>
# include <vector>
# include <iostream>
# include <fstream>
# include <assimp/scene.h>
# include "MiniEngine/Material.h"
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

		virtual bool                    loadFromAssimp(aiMesh *mesh, const std::vector<Material*> &materials);

		std::vector<Vertex>             vertexs;
		std::vector<unsigned int>       indices;

        unsigned int                    offset;
        unsigned int                    idxOffset;

        unsigned int                    materialId;
	};
}