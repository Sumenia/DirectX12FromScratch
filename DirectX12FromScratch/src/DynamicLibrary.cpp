#include "DynamicLibrary.h"

DynamicLibrary::DynamicLibrary() : _handle(nullptr)
{}

DynamicLibrary::~DynamicLibrary()
{
	if (_handle != nullptr)
	{
# if defined(SYSTEM_WINDOWS)
		FreeLibrary(_handle);
# elif defined(SYSTEM_LINUX)
		dlclose(_handle);
# endif
	}
}

bool DynamicLibrary::load(std::string const &filename)
{
# if defined(SYSTEM_WINDOWS)
	_handle = LoadLibrary(TEXT(filename.c_str()));
# elif defined(SYSTEM_UNIX)
	_handle = dlopen(filename.c_str(), RTLD_LAZY);
# endif

	return (_handle != nullptr);
}

void *DynamicLibrary::loadSymbol(std::string const &name)
{
	void    *symbol;

# if defined(SYSTEM_WINDOWS)
	symbol = GetProcAddress(_handle, name.c_str());
# elif defined(SYSTEM_UNIX)
	symbol = dlsym(_handle, name.c_str());
# endif

	return (symbol);
}

std::string DynamicLibrary::buildName(std::string const &name)
{
# if defined(SYSTEM_WINDOWS)
	return (name + ".dll");
# elif defined(SYSTEM_APPLE)
	return ("lib" + name + ".dylib");
# elif defined(SYSTEM_LINUX)
	return ("lib" + name + ".so");
# endif

	return ("");
}