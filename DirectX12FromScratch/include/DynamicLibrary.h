#ifndef DYNAMIC_LIBRARY_H_
# define DYNAMIC_LIBRARY_H_

# include "Config.h"

# if defined(SYSTEM_WINDOWS)

#include <windows.h>

typedef HINSTANCE   DynamicLibraryHandle;

#define DLL_EXPORT __declspec(dllexport)

# elif defined(SYSTEM_UNIX)

#include <dlfcn.h>
#include <unistd.h>

typedef void*       DynamicLibraryHandle;

#define DLL_EXPORT

# endif

# include <string>
# include <functional>

class DynamicLibrary
{
public:
	DynamicLibrary();
	~DynamicLibrary();

	bool                    load(std::string const &filename);
	void                    *loadSymbol(std::string const &name);

	static std::string      buildName(std::string const &name);

protected:
	DynamicLibraryHandle    _handle;
};

#endif