#ifndef CONFIG_H_
# define CONFIG_H_

# if defined(_WIN32)
#  define SYSTEM_WINDOWS
# elif defined(__unix__)
# define SYSTEM_UNIX
#	if defined(__linux__)
#		define SYSTEM_LINUX
#	elif define(__APPLE__)
#		define SYSTEM_APPLE
#	endif
# endif
#endif