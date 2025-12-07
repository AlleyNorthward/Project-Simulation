#ifndef _ENGLISH_HPP
#define _ENGLISH_HPP

#ifdef _WIN32
    #ifdef courseLibExport
        #define COURSE_LIB_API __declspec(dllexport)
    #else
        #define COURSE_LIB_API __declspec(dllimport)
    #endif
#else
    #define COURSE_LIB_API
#endif

COURSE_LIB_API void english();
#endif













