/*  DO NOT EDIT THIS FILE.

    It has been auto-edited by fixincludes from:

	"/Developer/SDKs/MacOSX10.3.9.sdk/usr/include/fenv.h"

    This had to be done to correct non-standard usages in the
    original, manufacturer supplied header file.  */

/* This file is a backwards compability hack to allow 
   no-trapping-math to be the default.  */
#ifndef _DARWIN_FENV_H_WRAPPER
#if defined(__GNUC__) && __GNUC__ >= 4
#pragma GCC fenv
#endif
#include_next <fenv.h>
#define _DARWIN_FENV_H_WRAPPER
#endif /* _DARWIN_FENV_H_WRAPPER */