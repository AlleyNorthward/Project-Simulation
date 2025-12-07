#ifndef _MATH_HPP
#define _MATH_HPP

#ifdef _WIN32
    #ifdef courseLibExport
        #define COURSE_LIB_API __declspec(dllexport)
    #else
        #define COURSE_LIB_API __declspec(dllimport)
    #endif
#else
    #define COURSE_LIB_API
#endif

void math();
#endif










