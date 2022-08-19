/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 4.0.2
 *
 * This file is not intended to be easily readable and contains a number of
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG
 * interface file instead.
 * ----------------------------------------------------------------------------- */


#ifndef SWIGJAVA
#define SWIGJAVA
#endif


/* -----------------------------------------------------------------------------
 *  This section contains generic SWIG labels for method/variable
 *  declarations/attributes, and other compiler dependent labels.
 * ----------------------------------------------------------------------------- */

/* template workaround for compilers that cannot correctly implement the C++ standard */
#ifndef SWIGTEMPLATEDISAMBIGUATOR
# if defined(__SUNPRO_CC) && (__SUNPRO_CC <= 0x560)
#  define SWIGTEMPLATEDISAMBIGUATOR template
# elif defined(__HP_aCC)
/* Needed even with `aCC -AA' when `aCC -V' reports HP ANSI C++ B3910B A.03.55 */
/* If we find a maximum version that requires this, the test would be __HP_aCC <= 35500 for A.03.55 */
#  define SWIGTEMPLATEDISAMBIGUATOR template
# else
#  define SWIGTEMPLATEDISAMBIGUATOR
# endif
#endif

/* inline attribute */
#ifndef SWIGINLINE
# if defined(__cplusplus) || (defined(__GNUC__) && !defined(__STRICT_ANSI__))
#   define SWIGINLINE inline
# else
#   define SWIGINLINE
# endif
#endif

/* attribute recognised by some compilers to avoid 'unused' warnings */
#ifndef SWIGUNUSED
# if defined(__GNUC__)
#   if !(defined(__cplusplus)) || (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4))
#     define SWIGUNUSED __attribute__ ((__unused__))
#   else
#     define SWIGUNUSED
#   endif
# elif defined(__ICC)
#   define SWIGUNUSED __attribute__ ((__unused__))
# else
#   define SWIGUNUSED
# endif
#endif

#ifndef SWIG_MSC_UNSUPPRESS_4505
# if defined(_MSC_VER)
#   pragma warning(disable : 4505) /* unreferenced local function has been removed */
# endif
#endif

#ifndef SWIGUNUSEDPARM
# ifdef __cplusplus
#   define SWIGUNUSEDPARM(p)
# else
#   define SWIGUNUSEDPARM(p) p SWIGUNUSED
# endif
#endif

/* internal SWIG method */
#ifndef SWIGINTERN
# define SWIGINTERN static SWIGUNUSED
#endif

/* internal inline SWIG method */
#ifndef SWIGINTERNINLINE
# define SWIGINTERNINLINE SWIGINTERN SWIGINLINE
#endif

/* exporting methods */
#if defined(__GNUC__)
#  if (__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
#    ifndef GCC_HASCLASSVISIBILITY
#      define GCC_HASCLASSVISIBILITY
#    endif
#  endif
#endif

#ifndef SWIGEXPORT
# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   if defined(STATIC_LINKED)
#     define SWIGEXPORT
#   else
#     define SWIGEXPORT __declspec(dllexport)
#   endif
# else
#   if defined(__GNUC__) && defined(GCC_HASCLASSVISIBILITY)
#     define SWIGEXPORT __attribute__ ((visibility("default")))
#   else
#     define SWIGEXPORT
#   endif
# endif
#endif

/* calling conventions for Windows */
#ifndef SWIGSTDCALL
# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   define SWIGSTDCALL __stdcall
# else
#   define SWIGSTDCALL
# endif
#endif

/* Deal with Microsoft's attempt at deprecating C standard runtime functions */
#if !defined(SWIG_NO_CRT_SECURE_NO_DEPRECATE) && defined(_MSC_VER) && !defined(_CRT_SECURE_NO_DEPRECATE)
# define _CRT_SECURE_NO_DEPRECATE
#endif

/* Deal with Microsoft's attempt at deprecating methods in the standard C++ library */
#if !defined(SWIG_NO_SCL_SECURE_NO_DEPRECATE) && defined(_MSC_VER) && !defined(_SCL_SECURE_NO_DEPRECATE)
# define _SCL_SECURE_NO_DEPRECATE
#endif

/* Deal with Apple's deprecated 'AssertMacros.h' from Carbon-framework */
#if defined(__APPLE__) && !defined(__ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES)
# define __ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES 0
#endif

/* Intel's compiler complains if a variable which was never initialised is
 * cast to void, which is a common idiom which we use to indicate that we
 * are aware a variable isn't used.  So we just silence that warning.
 * See: https://github.com/swig/swig/issues/192 for more discussion.
 */
#ifdef __INTEL_COMPILER
# pragma warning disable 592
#endif


/* Fix for jlong on some versions of gcc on Windows */
#if defined(__GNUC__) && !defined(__INTEL_COMPILER)
  typedef long long __int64;
#endif

/* Fix for jlong on 64-bit x86 Solaris */
#if defined(__x86_64)
# ifdef _LP64
#   undef _LP64
# endif
#endif

#include <jni.h>
#include <stdlib.h>
#include <string.h>


/* Support for throwing Java exceptions */
typedef enum {
  SWIG_JavaOutOfMemoryError = 1,
  SWIG_JavaIOException,
  SWIG_JavaRuntimeException,
  SWIG_JavaIndexOutOfBoundsException,
  SWIG_JavaArithmeticException,
  SWIG_JavaIllegalArgumentException,
  SWIG_JavaNullPointerException,
  SWIG_JavaDirectorPureVirtual,
  SWIG_JavaUnknownError,
  SWIG_JavaIllegalStateException,
} SWIG_JavaExceptionCodes;

typedef struct {
  SWIG_JavaExceptionCodes code;
  const char *java_exception;
} SWIG_JavaExceptions_t;


static void SWIGUNUSED SWIG_JavaThrowException(JNIEnv *jenv, SWIG_JavaExceptionCodes code, const char *msg) {
  jclass excep;
  static const SWIG_JavaExceptions_t java_exceptions[] = {
    { SWIG_JavaOutOfMemoryError, "java/lang/OutOfMemoryError" },
    { SWIG_JavaIOException, "java/io/IOException" },
    { SWIG_JavaRuntimeException, "java/lang/RuntimeException" },
    { SWIG_JavaIndexOutOfBoundsException, "java/lang/IndexOutOfBoundsException" },
    { SWIG_JavaArithmeticException, "java/lang/ArithmeticException" },
    { SWIG_JavaIllegalArgumentException, "java/lang/IllegalArgumentException" },
    { SWIG_JavaNullPointerException, "java/lang/NullPointerException" },
    { SWIG_JavaDirectorPureVirtual, "java/lang/RuntimeException" },
    { SWIG_JavaUnknownError,  "java/lang/UnknownError" },
    { SWIG_JavaIllegalStateException, "java/lang/IllegalStateException" },
    { (SWIG_JavaExceptionCodes)0,  "java/lang/UnknownError" }
  };
  const SWIG_JavaExceptions_t *except_ptr = java_exceptions;

  while (except_ptr->code != code && except_ptr->code)
    except_ptr++;

  (*jenv)->ExceptionClear(jenv);
  excep = (*jenv)->FindClass(jenv, except_ptr->java_exception);
  if (excep)
    (*jenv)->ThrowNew(jenv, excep, msg);
}


/* Contract support */

#define SWIG_contract_assert(nullreturn, expr, msg) if (!(expr)) {SWIG_JavaThrowException(jenv, SWIG_JavaIllegalArgumentException, msg); return nullreturn; } else


#include "encryption.h"


#ifdef __cplusplus
extern "C" {
#endif

SWIGEXPORT void JNICALL Java_com_volcengine_secureunionid_SecureUnionIDJNI_HASHIT(JNIEnv *jenv, jclass jcls, jbyteArray jarg1, jstring jarg2) {
  char *arg1 = (char *) 0 ;
  char *arg2 = (char *) 0 ;
  
  (void)jenv;
  (void)jcls;
  arg1 = 0;
  {
    arg1 = (char *)(*jenv)->GetByteArrayElements(jenv, jarg1, 0); 
  }
  arg2 = 0;
  if (jarg2) {
    arg2 = (char *)(*jenv)->GetStringUTFChars(jenv, jarg2, 0);
    if (!arg2) return ;
  }
  HASHIT(arg1,arg2);
  {
    (*jenv)->ReleaseByteArrayElements(jenv, jarg1, (jbyte *) arg1, 0); 
  }
  if (arg2) (*jenv)->ReleaseStringUTFChars(jenv, jarg2, (const char *)arg2);
}


SWIGEXPORT jlong JNICALL Java_com_volcengine_secureunionid_SecureUnionIDJNI_randomSeed(JNIEnv *jenv, jclass jcls) {
  jlong jresult = 0 ;
  unsigned long result;
  
  (void)jenv;
  (void)jcls;
  result = (unsigned long)randomSeed();
  jresult = (jlong)result; 
  return jresult;
}

SWIGEXPORT jint JNICALL Java_com_volcengine_secureunionid_SecureUnionIDJNI_genRandSeed(JNIEnv *jenv, jclass jcls, jbyteArray jarg1) {
  jint jresult = 0 ;
  char *arg1 = (char *) 0 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  {
    arg1 = (char *) (*jenv)->GetByteArrayElements(jenv, jarg1, 0); 
  }
  result = (int)genRandSeed(arg1);
  jresult = (jint)result; 
  {
    (*jenv)->ReleaseByteArrayElements(jenv, jarg1, (jbyte *) arg1, 0); 
  }
  
  return jresult;
}

SWIGEXPORT jint JNICALL Java_com_volcengine_secureunionid_SecureUnionIDJNI_genMasterKey(JNIEnv *jenv, jclass jcls, jbyteArray jarg1, jbyteArray jarg2) {
  jint jresult = 0 ;
  char *arg1 = (char *) 0 ;
  char *arg2 = (char *) 0 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  {
    arg1 = (char *) (*jenv)->GetByteArrayElements(jenv, jarg1, 0);
    arg2 = (char *) (*jenv)->GetByteArrayElements(jenv, jarg2, 0); 
  }
  result = (int)genMasterKey(arg1,arg2);
  jresult = (jint)result; 
  {
    (*jenv)->ReleaseByteArrayElements(jenv, jarg1, (jbyte *) arg1, 0); 
    (*jenv)->ReleaseByteArrayElements(jenv, jarg2, (jbyte *) arg2, 0); 
  }
  
  return jresult;
}

SWIGEXPORT jint JNICALL Java_com_volcengine_secureunionid_SecureUnionIDJNI_MasterKeygen(JNIEnv *jenv, jclass jcls, jlong jarg1, jbyteArray jarg2) {
  jint jresult = 0 ;
  unsigned long arg1 ;
  char *arg2 = (char *) 0 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  arg1 = (unsigned long)jarg1; 
  {
    arg2 = (char *) (*jenv)->GetByteArrayElements(jenv, jarg2, 0); 
  }
  result = (int)MasterKeygen(arg1,arg2);
  jresult = (jint)result; 
  {
    (*jenv)->ReleaseByteArrayElements(jenv, jarg2, (jbyte *) arg2, 0); 
  }
  
  return jresult;
}


SWIGEXPORT jint JNICALL Java_com_volcengine_secureunionid_SecureUnionIDJNI_Keygen(JNIEnv *jenv, jclass jcls, jbyteArray jarg1, jstring jarg2, jbyteArray jarg3, jbyteArray jarg4, jbyteArray jarg5) {
  jint jresult = 0 ;
  char *arg1 = (char *) 0 ;
  char *arg2 = (char *) 0 ;
  char *arg3 = (char *) 0 ;
  char *arg4 = (char *) 0 ;
  char *arg5 = (char *) 0 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  {
    arg1 = (char *) (*jenv)->GetByteArrayElements(jenv, jarg1, 0); 
  }
  arg2 = 0;
  if (jarg2) {
    arg2 = (char *)(*jenv)->GetStringUTFChars(jenv, jarg2, 0);
    if (!arg2) return 0;
  }
  {
    arg3 = (char *) (*jenv)->GetByteArrayElements(jenv, jarg3, 0); 
  }
  {
    arg4 = (char *) (*jenv)->GetByteArrayElements(jenv, jarg4, 0); 
  }
  {
    arg5 = (char *) (*jenv)->GetByteArrayElements(jenv, jarg5, 0); 
  }
  result = (int)Keygen(arg1,arg2,arg3,arg4,arg5);
  jresult = (jint)result; 
  {
    (*jenv)->ReleaseByteArrayElements(jenv, jarg1, (jbyte *) arg1, 0); 
  }
  {
    (*jenv)->ReleaseByteArrayElements(jenv, jarg3, (jbyte *) arg3, 0); 
  }
  {
    (*jenv)->ReleaseByteArrayElements(jenv, jarg4, (jbyte *) arg4, 0); 
  }
  {
    (*jenv)->ReleaseByteArrayElements(jenv, jarg5, (jbyte *) arg5, 0); 
  }
  
  if (arg2) (*jenv)->ReleaseStringUTFChars(jenv, jarg2, (const char *)arg2);
  
  
  
  return jresult;
}


SWIGEXPORT jint JNICALL Java_com_volcengine_secureunionid_SecureUnionIDJNI_System_1Keygen(JNIEnv *jenv, jclass jcls, jobjectArray jarg1, jobjectArray jarg2, jint jarg3, jbyteArray jarg4, jbyteArray jarg5) {
  jint jresult = 0 ;
  char **arg1 = (char **) 0 ;
  char **arg2 = (char **) 0 ;
  int arg3 ;
  char *arg4 = (char *) 0 ;
  char *arg5 = (char *) 0 ;
  jint size1 ;
  jint size2 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  {
    int i = 0;
    if (jarg1) {
      size1 = (*jenv)->GetArrayLength(jenv, jarg1);
      
      
      
      arg1 = (char **)malloc((size1+1) * sizeof(char *));
      
      for (i = 0; i<size1; i++) {
        jstring j_string = (jstring)(*jenv)->GetObjectArrayElement(jenv, jarg1, i);
        const char *c_string = (*jenv)->GetStringUTFChars(jenv, j_string, 0);
        
        
        
        arg1[i] = (char *)malloc((strlen(c_string)+1) * sizeof(const char *));
        
        strcpy(arg1[i], c_string);
        (*jenv)->ReleaseStringUTFChars(jenv, j_string, c_string);
        (*jenv)->DeleteLocalRef(jenv, j_string);
      }
      arg1[i] = 0;
    } else {
      arg1 = 0;
      size1 = 0;
    }
  }
  {
    int i = 0;
    if (jarg2) {
      size2 = (*jenv)->GetArrayLength(jenv, jarg2);
      
      
      
      arg2 = (char **)malloc((size2+1) * sizeof(char *));
      
      for (i = 0; i<size2; i++) {
        jstring j_string = (jstring)(*jenv)->GetObjectArrayElement(jenv, jarg2, i);
        const char *c_string = (*jenv)->GetStringUTFChars(jenv, j_string, 0);
        
        
        
        arg2[i] = (char *)malloc((strlen(c_string)+1) * sizeof(const char *));
        
        strcpy(arg2[i], c_string);
        (*jenv)->ReleaseStringUTFChars(jenv, j_string, c_string);
        (*jenv)->DeleteLocalRef(jenv, j_string);
      }
      arg2[i] = 0;
    } else {
      arg2 = 0;
      size2 = 0;
    }
  }
  arg3 = (int)jarg3; 
  {
    arg4 = (char *) (*jenv)->GetByteArrayElements(jenv, jarg4, 0); 
  }
  {
    arg5 = (char *) (*jenv)->GetByteArrayElements(jenv, jarg5, 0); 
  }
  result = (int)System_Keygen(arg1,arg2,arg3,arg4,arg5);
  jresult = (jint)result; 
  {
    (*jenv)->ReleaseByteArrayElements(jenv, jarg4, (jbyte *) arg4, 0); 
  }
  {
    (*jenv)->ReleaseByteArrayElements(jenv, jarg5, (jbyte *) arg5, 0); 
  }
  {
    int i;
    for (i=0; i<size1; i++)
    
    
    
    
    free(arg1[i]);
    free(arg1);
    
  }
  {
    int i;
    for (i=0; i<size2; i++)
    
    
    
    
    free(arg2[i]);
    free(arg2);
    
  }
  
  
  return jresult;
}


SWIGEXPORT jint JNICALL Java_com_volcengine_secureunionid_SecureUnionIDJNI_Blinding(JNIEnv *jenv, jclass jcls, jstring jarg1, jlong jarg2, jbyteArray jarg3, jbyteArray jarg4) {
  jint jresult = 0 ;
  char *arg1 = (char *) 0 ;
  unsigned long arg2 ;
  char *arg3 = (char *) 0 ;
  char *arg4 = (char *) 0 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  arg1 = 0;
  if (jarg1) {
    arg1 = (char *)(*jenv)->GetStringUTFChars(jenv, jarg1, 0);
    if (!arg1) return 0;
  }
  arg2 = (unsigned long)jarg2; 
  {
    arg3 = (char *) (*jenv)->GetByteArrayElements(jenv, jarg3, 0); 
  }
  {
    arg4 = (char *) (*jenv)->GetByteArrayElements(jenv, jarg4, 0); 
  }
  result = (int)Blinding(arg1,arg2,arg3,arg4);
  jresult = (jint)result; 
  {
    (*jenv)->ReleaseByteArrayElements(jenv, jarg3, (jbyte *) arg3, 0); 
  }
  {
    (*jenv)->ReleaseByteArrayElements(jenv, jarg4, (jbyte *) arg4, 0); 
  }
  if (arg1) (*jenv)->ReleaseStringUTFChars(jenv, jarg1, (const char *)arg1);
  
  
  return jresult;
}


SWIGEXPORT jint JNICALL Java_com_volcengine_secureunionid_SecureUnionIDJNI_Enc(JNIEnv *jenv, jclass jcls, jbyteArray jarg1, jbyteArray jarg2, jbyteArray jarg3) {
  jint jresult = 0 ;
  char *arg1 = (char *) 0 ;
  char *arg2 = (char *) 0 ;
  char *arg3 = (char *) 0 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  {
    arg1 = (char *) (*jenv)->GetByteArrayElements(jenv, jarg1, 0); 
  }
  {
    arg2 = (char *) (*jenv)->GetByteArrayElements(jenv, jarg2, 0); 
  }
  {
    arg3 = (char *) (*jenv)->GetByteArrayElements(jenv, jarg3, 0); 
  }
  result = (int)Enc(arg1,arg2,arg3);
  jresult = (jint)result; 
  {
    (*jenv)->ReleaseByteArrayElements(jenv, jarg1, (jbyte *) arg1, 0); 
  }
  {
    (*jenv)->ReleaseByteArrayElements(jenv, jarg2, (jbyte *) arg2, 0); 
  }
  {
    (*jenv)->ReleaseByteArrayElements(jenv, jarg3, (jbyte *) arg3, 0); 
  }
  
  
  
  return jresult;
}


SWIGEXPORT jint JNICALL Java_com_volcengine_secureunionid_SecureUnionIDJNI_Unblinding(JNIEnv *jenv, jclass jcls, jobjectArray jarg1, jint jarg2, jbyteArray jarg3, jbyteArray jarg4, jbyteArray jarg5) {
  jint jresult = 0 ;
  char **arg1 = (char **) 0 ;
  int arg2 ;
  char *arg3 = (char *) 0 ;
  char *arg4 = (char *) 0 ;
  char *arg5 = (char *) 0 ;
  jint size1 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  {
    int i = 0;
    if (jarg1) {
      size1 = (*jenv)->GetArrayLength(jenv, jarg1);
      
      
      
      arg1 = (char **)malloc((size1+1) * sizeof(char *));
      
      for (i = 0; i<size1; i++) {
        jstring j_string = (jstring)(*jenv)->GetObjectArrayElement(jenv, jarg1, i);
        const char *c_string = (*jenv)->GetStringUTFChars(jenv, j_string, 0);
        
        
        
        arg1[i] = (char *)malloc((strlen(c_string)+1) * sizeof(const char *));
        
        strcpy(arg1[i], c_string);
        (*jenv)->ReleaseStringUTFChars(jenv, j_string, c_string);
        (*jenv)->DeleteLocalRef(jenv, j_string);
      }
      arg1[i] = 0;
    } else {
      arg1 = 0;
      size1 = 0;
    }
  }
  arg2 = (int)jarg2; 
  {
    arg3 = (char *) (*jenv)->GetByteArrayElements(jenv, jarg3, 0); 
  }
  {
    arg4 = (char *) (*jenv)->GetByteArrayElements(jenv, jarg4, 0); 
  }
  {
    arg5 = (char *) (*jenv)->GetByteArrayElements(jenv, jarg5, 0); 
  }
  result = (int)Unblinding(arg1,arg2,arg3,arg4,arg5);
  jresult = (jint)result; 
  {
    (*jenv)->ReleaseByteArrayElements(jenv, jarg3, (jbyte *) arg3, 0); 
  }
  {
    (*jenv)->ReleaseByteArrayElements(jenv, jarg4, (jbyte *) arg4, 0); 
  }
  {
    (*jenv)->ReleaseByteArrayElements(jenv, jarg5, (jbyte *) arg5, 0); 
  }
  {
    int i;
    for (i=0; i<size1; i++)
    
    
    
    
    free(arg1[i]);
    free(arg1);
    
  }
  
  
  
  return jresult;
}

SWIGEXPORT jint JNICALL Java_com_volcengine_secureunionid_SecureUnionIDJNI_Blind(JNIEnv *jenv, jclass jcls, jstring jarg1, jbyteArray jarg2, jbyteArray jarg3, jbyteArray jarg4) {
  jint jresult = 0 ;
  char *arg1 = (char *) 0 ;
  char *arg2 = (char *) 0 ;
  char *arg3 = (char *) 0 ;
  char *arg4 = (char *) 0 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  arg1 = 0;
  if (jarg1) {
    arg1 = (char *)(*jenv)->GetStringUTFChars(jenv, jarg1, 0);
    if (!arg1) return 0;
  }
  {
    arg2 = (char *) (*jenv)->GetByteArrayElements(jenv, jarg2, 0);
  } 
  {
    arg3 = (char *) (*jenv)->GetByteArrayElements(jenv, jarg3, 0); 
  }
  {
    arg4 = (char *) (*jenv)->GetByteArrayElements(jenv, jarg4, 0); 
  }
  result = (int)Blind(arg1,arg2,arg3,arg4);
  jresult = (jint)result; 
    {
    (*jenv)->ReleaseByteArrayElements(jenv, jarg2, (jbyte *) arg2, 0); 
  }
  {
    (*jenv)->ReleaseByteArrayElements(jenv, jarg3, (jbyte *) arg3, 0); 
  }
  {
    (*jenv)->ReleaseByteArrayElements(jenv, jarg4, (jbyte *) arg4, 0); 
  }
  if (arg1) (*jenv)->ReleaseStringUTFChars(jenv, jarg1, (const char *)arg1);
  
  
  return jresult;
}

#ifdef __cplusplus
}
#endif

