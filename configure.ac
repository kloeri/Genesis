dnl Process this file with autoconf to produce a configure script.

AC_INIT([genesis], [0.0], [http://bugs.exherbo.org])

AC_CONFIG_SRCDIR([src/genesis.cc])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.11.1 dist-bzip2 no-dist-gzip])

AC_LANG([C++])

AC_PROG_CXX
AC_PROG_INSTALL

LT_INIT

AC_CHECK_PROG(HAVE_PRCEPLUSPLUS, [pcre++-config], [yes], [no])
if test x"$HAVE_PRCEPLUSPLUS" = "xyes" ; then
    PCREPLUSPLUS_CFLAGS=`pcre++-config --cflags`
    AC_SUBST(PCREPLUSPLUS_CFLAGS)
    PCREPLUSPLUS_LIBS=`pcre++-config --libs`
    AC_SUBST(PCREPLUSPLUS_LIBS)
else
    AC_MSG_ERROR([pcre++ (http://www.daemon.de/PCRE) is required])
fi

AC_CONFIG_FILES([
   Makefile
   scripts/Makefile
   src/Makefile
   src/actions/Makefile
   src/events/Makefile
])

AC_OUTPUT

