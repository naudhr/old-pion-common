# --------------------------------------
# pion-setup autoconf configuration file
# --------------------------------------

# DO NOT USE autoheader (the .hpp.in file is not automanaged)
AUTOHEADER="echo autoheader ignored"

# Check for programs
AC_PROG_CXX
AC_PROG_INSTALL

# Use C++ language
AC_LANG_CPLUSPLUS
AX_COMPILER_VENDOR

# Set basic compiler options
case "$build_os" in
*solaris*)
	if test "$ax_cv_cxx_compiler_vendor" = "sun"; then
		# Solaris: Sun Studio C++ compiler
		CPPFLAGS="$CPPFLAGS -mt -library=stlport4"
		LDFLAGS="$LDFLAGS -mt -library=stlport4"
		PION_OPT_FLAGS="-O2 -DNDEBUG"
		PION_DEBUG_FLAGS="-g"
	else
		# Solaris: GCC compiler
		CPPFLAGS="$CPPFLAGS -pthreads -D_REENTRANT"
		LDFLAGS="$LDFLAGS -pthreads"
		PION_OPT_FLAGS="-O2 -Wall -Wno-strict-aliasing -DNDEBUG"
		PION_DEBUG_FLAGS="-ggdb -Wall -Wno-strict-aliasing -fkeep-inline-functions"
	fi
	PION_EXTERNAL_LIBS="-lsocket -lnsl -ldl -lposix4"
	;;
*darwin*)
	# Mac OS X: GCC compiler
	# -pthread is implied (automatically set by compiler)
	CPPFLAGS="$CPPFLAGS -D_REENTRANT"
	#LDFLAGS="$LDFLAGS"
	PION_OPT_FLAGS="-fast -Wall -Wno-strict-aliasing -DNDEBUG"
	PION_DEBUG_FLAGS="-ggdb -Wall -Wno-strict-aliasing -fkeep-inline-functions"
	PION_EXTERNAL_LIBS="-ldl"
	;;
*cygwin*)
	# Cygwin GCC compiler (Windows)
	CPPFLAGS="$CPPFLAGS -mthreads -D_REENTRANT -DPION_WIN32 -D__USE_W32_SOCKETS"
	LDFLAGS="$LDFLAGS -mthreads -Wl,-E"
	PION_OPT_FLAGS="-O2 -Wall -Wno-strict-aliasing -DNDEBUG"
	PION_DEBUG_FLAGS="-ggdb -Wall -Wno-strict-aliasing -fkeep-inline-functions"
	PION_EXTERNAL_LIBS="-lws2_32 -lmswsock"
	;;
*freebsd*)
	# FreeBSD: GCC compiler
	CPPFLAGS="$CPPFLAGS -pthread -D_REENTRANT"
	LDFLAGS="$LDFLAGS -pthread -Wl,-E"
	PION_OPT_FLAGS="-O2 -Wall -Wno-strict-aliasing -DNDEBUG"
	PION_DEBUG_FLAGS="-ggdb -Wall -Wno-strict-aliasing -fkeep-inline-functions"
	;;
*)
	# Other (Linux): GCC compiler
	CPPFLAGS="$CPPFLAGS -pthread -D_REENTRANT"
	LDFLAGS="$LDFLAGS -pthread -Wl,-E"
	PION_OPT_FLAGS="-O2 -Wall -Wno-strict-aliasing -DNDEBUG"
	PION_DEBUG_FLAGS="-ggdb -Wall -Wno-strict-aliasing -fkeep-inline-functions"
	PION_EXTERNAL_LIBS="-ldl"
	;;
esac


# Check for debug
AC_MSG_CHECKING([for debugging])
AC_ARG_WITH([debug],
    AC_HELP_STRING([--with-debug],[build with debugging information]),
    [with_debug=$withval],
    [with_debug=no])
if test "$with_debug" = "no"; then
	AC_MSG_RESULT(no)
	CFLAGS="$CFLAGS $PION_OPT_FLAGS"
	CXXFLAGS="$CXXFLAGS $PION_OPT_FLAGS"
else
	AC_MSG_RESULT(yes)
	CFLAGS="$CFLAGS $PION_DEBUG_FLAGS"
	CXXFLAGS="$CXXFLAGS $PION_DEBUG_FLAGS"
fi


# Check for --with-plugins
AC_MSG_CHECKING([for plug-ins directory])
AC_ARG_WITH([plugins],
    AC_HELP_STRING([--with-plugins@<:@=DIR@:>@],[directory where Pion Plug-ins are installed]),
    [with_plugins=$withval],
    [with_plugins=no])
if test "$with_plugins" = "no"; then
	if test "x$prefix" = "xNONE"; then
		PION_PLUGINS_DIRECTORY=/usr/local/lib/pion/plugins
	else
		PION_PLUGINS_DIRECTORY=${prefix}/lib/pion/plugins
	fi
else
	PION_PLUGINS_DIRECTORY=$with_plugins
fi
AC_MSG_RESULT($PION_PLUGINS_DIRECTORY)
AC_DEFINE_UNQUOTED([PION_PLUGINS_DIRECTORY],["$PION_PLUGINS_DIRECTORY"],[Define to the directory where Pion Plug-ins are installed.])
AC_SUBST(PION_PLUGINS_DIRECTORY)