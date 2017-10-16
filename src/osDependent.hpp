#ifndef OSDEPENDENT_HPP_
#define OSDEPENDENT_HPP_

#ifdef _WIN64
   #define SYSTEM "Windows"
#elif _WIN32
   #define SYSTEM "Windows"
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_MAC
        // Other kinds of Mac OS
    #endif
#endif

#endif /* OSDEPENDENT_HPP_ */
