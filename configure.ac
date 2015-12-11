#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([xfb], [1.0], [silent_dxx@163.com])
AC_CONFIG_SRCDIR([src/xfb.h])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([XFB], [1.0])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-ljpeg':
AC_CHECK_LIB([jpeg], [main])
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])
# FIXME: Replace `main' with a function in `-lncurses':
AC_CHECK_LIB([ncurses], [main])
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [main])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h sys/ioctl.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_CHECK_FUNCS([atexit getpagesize memmove memset munmap select])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
