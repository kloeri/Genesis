dnl Process this file with autoconf to produce a configure script.

AC_INIT([genesis], [0.1alpha0], [http://bugs.exherbo.org])

AC_CONFIG_SRCDIR([src/genesis-handler/genesis-handler.cc])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.11.1 dist-bzip2 no-dist-gzip subdir-objects])
AM_PROG_AR

AC_PROG_CXX
AX_CXX_COMPILE_STDCXX([14], [noext], [mandatory])
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

AC_SEARCH_LIBS([mnl_socket_open], [mnl], [], [
    AC_MSG_ERROR([unable to find the mnl_socket_open() function])
    ])

AC_CHECK_HEADERS_ONCE(libmnl/libmnl.h)

AM_SILENT_RULES([no])
AC_CONFIG_FILES([
   Makefile
])

AC_OUTPUT

