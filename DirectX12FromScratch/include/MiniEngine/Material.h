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
    class RenderSystem;

    class Material
    {
    public:
		enum Flag : DWORD64
		{
			// Method to retrieve the color of the fragment (default use kd)
			NORMAL_COLOR = 1,
			TEXTURE_DIFFUSE = 2,

			// Method to retrieve the normal of the vertice (default use normal arg)
			NORMAL_MAP = 4,

			// Method to retrieve the ambient color (default use ka)
			TEXTURE_AMBIENT = 8,

            // Method to retrieve the specular color (default use ks)
            TEXTURE_SPECULAR = 16,
        };

		enum TextureType
		{
            AMBIENT = 0,
			DIFFUSE = 1,
			SPECULAR = 2
		};

    public:
        Material(RenderSystem &system);
        virtual ~Material();

		bool			loadFromAssimp(aiMaterial* material, const std::string& path);

        void            useNormalColor();
        void            useDiffuseColor(Vector3f const &color);
        void            useAmbientColor(Vector3f const &color);
        void            useSpecularColor(Vector3f const &color);
        void            setShininess(float shininess);

        void            useTexture(TextureType t, Texture *tex);

        void            useNormalScalar();
        void            useNormalMap(/* NORMAL MAP */);

        virtual bool    bind(CommandList &list, unsigned int rootIdx) = 0;
        virtual bool    finalize() = 0;

        DWORD64         getFlags() const;
        unsigned int    getId() const;

        std::string     generateHLSLShader(Shader::Type type);

        bool            haveAmbientMap() const;
        bool            haveDiffuseMap() const;
        bool            haveSpecularMap() const;

    protected:
        void            setFlagToShader(std::stringstream &source, std::string const &define, Flag flag);

        RenderSystem    &_system;

        unsigned int    _id;
        DWORD64         _flags;

        Vector3f        _ka;
        Vector3f        _kd;
        Vector3f        _ks;
        float           _shininess;
		
        // Stock texture
		std::map<TextureType, Texture*> _textures;

        // Stock normal map

    protected:
        static unsigned int id_count;
    };
}