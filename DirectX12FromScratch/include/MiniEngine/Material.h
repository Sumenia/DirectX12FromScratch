#pragma once

#include <windows.h>
#include <map>
#include <vector>
#include <assimp/scene.h>
#include "MiniEngine/Texture.h"
#include "MiniEngine/Geometry.h"
#include "MiniEngine/Shader.h"

namespace MiniEngine
{
    class CommandList;

    // TO-DO: Add flags and behaviours to this class and the associated shader
    class Material
    {
    public:
        enum Flag : DWORD64
        {
            // Method to retrieve the color of the fragment
            NORMAL_COLOR = 0x1,
            UNIFORM_COLOR = 0x2,
            TEXTURE = 0x4,

            // Method to retrieve the normal of the vertice
            NORMAL_MAP = 0x8,
        };

		enum TextureType
		{
			DIFFUSE,
			SPECULAR,
			AMBIENT
		};

    public:
        Material();
        virtual ~Material();

		bool			loadFromAssimp(aiMaterial* material, const std::string& path);

        void            useNormalColor();
        void            useUniformColor(Vector3f const &color);
        void            useTexture(/* TEXTURE */);

        void            useNormalScalar();
        void            useNormalMap(/* NORMAL MAP */);

        virtual bool    bind(CommandList &list, unsigned int rootIdx) = 0;
        virtual bool    finalize() = 0;

        DWORD64         getFlags() const;
        unsigned int    getId() const;

        std::string     generateHLSLShader(Shader::Type type);

    protected:
        void            setFlagToShader(std::stringstream &source, std::string const &define, Flag flag);

        unsigned int    _id;
        DWORD64         _flags;

        Vector3f        _color;
		
        // Stock texture
		std::map<TextureType, Texture*> _textures;

        // Stock normal map

    protected:
        static unsigned int id_count;
    };
}