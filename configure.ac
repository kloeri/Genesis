dnl Process this file with autoconf to produce a configure script.

AC_INIT
AC_CONFIG_SRCDIR([src/genesis.cc])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE(genesis,0.0)
AC_PROG_CXX
AC_PROG_INSTALL
LT_INIT
AC_LANG(C++)
AC_CHECK_LIB([pthread],[main],[],[],[])ac_cv_lib_pthread=ac_cv_lib_pthread_main

AC_CHECK_LIB(pcre++,pcre_compile)

AC_CONFIG_FILES([Makefile scripts/Makefile src/Makefile src/actions/Makefile src/events/Makefile])
AC_OUTPUT