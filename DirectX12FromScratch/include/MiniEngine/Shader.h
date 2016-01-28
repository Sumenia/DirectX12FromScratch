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
        Shader(std::string const &filename, std::string const &entry);
        virtual ~Shader();

        virtual void        *getBytecode() = 0;
        virtual size_t      getSize() = 0;

        virtual bool        compile(Shader::Type) = 0;

    protected:
        std::string         _filename;
        std::string         _entry;
    };
}