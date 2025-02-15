//
// MATLAB Compiler: 24.1 (R2024a)
// Date: Thu Oct 24 14:44:30 2024
// Arguments:
// "-B""macro_default""-W""cpplib:euler_rotation_matrix,all,version=1.0""-T""lin
// k:lib""-d""D:\matlabLIB\euler_rotation_matrix\for_testing""-v""C:\Users\jack\
// Desktop\大三上\轨道力学及应用\作业\012\euler_rotation_matrix.m"
//

#ifndef euler_rotation_matrix_h
#define euler_rotation_matrix_h 1

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
#ifndef LIB_euler_rotation_matrix_C_API 
#define LIB_euler_rotation_matrix_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_euler_rotation_matrix_C_API 
bool MW_CALL_CONV euler_rotation_matrixInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_euler_rotation_matrix_C_API 
bool MW_CALL_CONV euler_rotation_matrixInitialize(void);
extern LIB_euler_rotation_matrix_C_API 
void MW_CALL_CONV euler_rotation_matrixTerminate(void);

extern LIB_euler_rotation_matrix_C_API 
void MW_CALL_CONV euler_rotation_matrixPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_euler_rotation_matrix_C_API 
bool MW_CALL_CONV mlxEuler_rotation_matrix(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                           *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_euler_rotation_matrix
#define PUBLIC_euler_rotation_matrix_CPP_API __declspec(dllexport)
#else
#define PUBLIC_euler_rotation_matrix_CPP_API __declspec(dllimport)
#endif

#define LIB_euler_rotation_matrix_CPP_API PUBLIC_euler_rotation_matrix_CPP_API

#else

#if !defined(LIB_euler_rotation_matrix_CPP_API)
#if defined(LIB_euler_rotation_matrix_C_API)
#define LIB_euler_rotation_matrix_CPP_API LIB_euler_rotation_matrix_C_API
#else
#define LIB_euler_rotation_matrix_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_euler_rotation_matrix_CPP_API void MW_CALL_CONV euler_rotation_matrix(int nargout, mwArray& R, const mwArray& rotations);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
