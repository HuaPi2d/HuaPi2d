//
// MATLAB Compiler: 24.1 (R2024a)
// Date: Sat Jun  8 14:57:52 2024
// Arguments:
// "-B""macro_default""-W""cpplib:Add,all,version=1.0""-T""link:lib""-d""D:\VS
// code\projects\mode recognition\experiment 4\Add\for_testing""-v""D:\VS
// code\projects\mode recognition\experiment 4\Add.m"
//

#ifndef Add_h
#define Add_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" { // sbcheck:ok:extern_c
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_Add_C_API 
#define LIB_Add_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_Add_C_API 
bool MW_CALL_CONV AddInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_Add_C_API 
bool MW_CALL_CONV AddInitialize(void);
extern LIB_Add_C_API 
void MW_CALL_CONV AddTerminate(void);

extern LIB_Add_C_API 
void MW_CALL_CONV AddPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_Add_C_API 
bool MW_CALL_CONV mlxAdd(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_Add
#define PUBLIC_Add_CPP_API __declspec(dllexport)
#else
#define PUBLIC_Add_CPP_API __declspec(dllimport)
#endif

#define LIB_Add_CPP_API PUBLIC_Add_CPP_API

#else

#if !defined(LIB_Add_CPP_API)
#if defined(LIB_Add_C_API)
#define LIB_Add_CPP_API LIB_Add_C_API
#else
#define LIB_Add_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_Add_CPP_API void MW_CALL_CONV Add(int nargout, mwArray& result, const mwArray& a, const mwArray& b);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
