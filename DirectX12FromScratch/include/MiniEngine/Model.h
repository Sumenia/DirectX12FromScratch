#pragma once

# include <list>
# include <memory>
# include "Mesh.h"

namespace MiniEngine
{
	class Model {
	public:
		Model();
		virtual ~Model();

		virtual bool loadObjFromFile(const std::string &file);
		bool isLoaded() const;
		unsigned int getVertexsSize() const;
		unsigned int getIndicesSize() const;
		const std::list<std::shared_ptr<Mesh> > &getMeshs();


	protected:
		std::list<std::shared_ptr<Mesh> >	_meshs;
		bool _isLoaded;
	};
}