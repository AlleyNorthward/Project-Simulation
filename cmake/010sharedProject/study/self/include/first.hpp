#ifndef _FIRST_HPP
#define _FIRST_HPP

#ifdef _WIN32
    #ifdef study_selfLibExport
        #define STUDY_SELF_LIB_API __declspec(dllexport)
    #else
        #define STUDY_SELF_LIB_API __declspec(dllimport)
    #endif
#else
    #define STUDY_SELF_LIB_API
#endif

STUDY_SELF_LIB_API void first();
#endif










