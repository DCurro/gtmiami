#ifndef OS_HPP_
#define OS_HPP_

#include <string>

#ifdef _WIN64
	#include <direct.h>
#elif _WIN32
    #include <direct.h>
	#include <io.h>
    #include <stdlib.h>
    #include <string.h>
#elif __APPLE__
	#include "TargetConditionals.h"
	#if TARGET_OS_MAC
		#include <mach-o/dyld.h>
	#endif
#endif

class OS {
	static std::string mPwd;

public:
	OS();
	virtual ~OS();

	static std::string getPwd();
};

#endif /* OS_HPP_ */
