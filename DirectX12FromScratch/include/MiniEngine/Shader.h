#pragma once

# include <string>

namespace MiniEngine
{
    class Shader
    {
    public:
        enum Type
        {
            VERTEX,
            PIXEL
        };

        struct Input
        {
            virtual ~Input() {};
        };

    public:
        Shader();
        virtual ~Shader();

        virtual void        *getBytecode() = 0;
        virtual size_t      getSize() = 0;

        virtual bool        compile(Shader::Type type, std::string const &source, std::string const &entry, std::string const &filename = "./Assets/shaders/main") = 0;
        virtual bool        compileFromFile(Shader::Type, std::string const &filename, std::string const &entry) = 0;
    };
}