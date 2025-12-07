#ifndef _WANG_HPP
#define _WANG_HPP

#ifdef _WIN32
    #ifdef nameLibExport
        #define NAME_LIB_API __declspec(dllexport)
    #else
        #define NAME_LIB_API __declspec(dllimport)
    #endif
#else
    #define NAME_LIB_API
#endif

NAME_LIB_API void wang();
#endif











