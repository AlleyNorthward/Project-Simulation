#ifndef _FOURTH_HPP
#define _FOURTH_HPP

#ifdef _WIN32
    #ifdef study_togetherLibExport
        #define STUDY_TOGETHER_LIB_API __declspec(dllexport)
    #else
        #define STUDY_TOGETHER_LIB_API __declspec(dllimport)
    #endif
#else
    #define STUDY_TOGETHER_LIB_API
#endif

STUDY_TOGETHER_LIB_API void fourth();
#endif











