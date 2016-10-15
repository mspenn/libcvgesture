#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef WIN32
# ifndef BUILDING_CVG_EXTENSION
#   define CVG_EXTERN __declspec(dllexport)
# else
#   define CVG_EXTERN __declspec(dllimport)
# endif
#endif

#endif