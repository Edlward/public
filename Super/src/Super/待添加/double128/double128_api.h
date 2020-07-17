#ifndef _DOUBLE128_API_H
#define _DOUBLE128_API_H

#ifdef DOUBLE128PRJ_EXPORTS
#define DOUBLE128PRJ_API __declspec(dllexport)
#else
#define DOUBLE128PRJ_API __declspec(dllimport)
#endif

#endif