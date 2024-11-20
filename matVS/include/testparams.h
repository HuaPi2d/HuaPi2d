//
// MATLAB Compiler: 24.1 (R2024a)
// Date: Thu Oct 31 15:31:08 2024
// Arguments:
// "-B""macro_default""-W""cpplib:testparams,all,version=1.0""-T""link:lib""-d""
// D:\matlabLIB\testparams\for_testing""-v""D:\matlabLIB\testparams.m"
//

#ifndef testparams_h
#define testparams_h 1

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
#ifndef LIB_testparams_C_API 
#define LIB_testparams_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_testparams_C_API 
bool MW_CALL_CONV testparamsInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_testparams_C_API 
bool MW_CALL_CONV testparamsInitialize(void);
extern LIB_testparams_C_API 
void MW_CALL_CONV testparamsTerminate(void);

extern LIB_testparams_C_API 
void MW_CALL_CONV testparamsPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_testparams_C_API 
bool MW_CALL_CONV mlxTestparams(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_testparams
#define PUBLIC_testparams_CPP_API __declspec(dllexport)
#else
#define PUBLIC_testparams_CPP_API __declspec(dllimport)
#endif

#define LIB_testparams_CPP_API PUBLIC_testparams_CPP_API

#else

#if !defined(LIB_testparams_CPP_API)
#if defined(LIB_testparams_C_API)
#define LIB_testparams_CPP_API LIB_testparams_C_API
#else
#define LIB_testparams_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_testparams_CPP_API void MW_CALL_CONV testparams(const mwArray& p);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
