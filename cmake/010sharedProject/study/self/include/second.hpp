#ifndef _SECOND_HPP
#define _SECOND_HPP

#ifdef _WIN32
    #ifdef study_selfLibExport
        #define STUDY_SELF_LIB_API __declspec(dllexport)
    #else
        #define STUDY_SELF_LIB_API __declspec(dllimport)
    #endif
#else
    #define STUDY_SELF_LIB_API
#endif

STUDY_SELF_LIB_API void second();
#endif











