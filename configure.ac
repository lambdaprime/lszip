#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([lszip], [1.0], [ID])
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB(z, main, , AC_MSG_ERROR([cant find zlib library]))

# Checks for header files.
AC_CHECK_HEADERS([memory.h stddef.h stdlib.h string.h])
AC_CHECK_HEADERS([zlib.h, , AC_MSG_ERROR([cant find zlib headers])])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset regcomp strcspn strrchr])

AM_ZLIB_VERSION_GT_1_2(, exit)

AC_OUTPUT(Makefile src/Makefile)
