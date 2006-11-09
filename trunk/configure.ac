dnl Process this file with autoconf to produce a configure script.
AC_INIT(calo, 0.25.1, ralf@ark.in-berlin.de)
AC_CONFIG_SRCDIR([src/TextDumper.cpp])

AC_PREREQ(2.52)

AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(config.h)

dnl  AC_SUBST(PACKAGE_BUGREPORT, bug-gnu-hello@gnu.org)
dnl AH_TEMPLATE(PACKAGE_BUGREPORT)
dnl AC_DEFINE_UNQUOTED(PACKAGE_BUGREPORT, "$PACKAGE_BUGREPORT")

dnl Set of available languages now in po/LINGUAS.

dnl -------------------------Checks for programs.
AC_PROG_CXX

dnl Check for build configuration.
AC_PROG_LIBTOOL

dnl gettext stuff
ALL_LINGUAS="en"
AC_SUBST(ALL_LINGUAS)

dnl Check for lynx
AC_PATH_PROG(LYNX_PATH,lynx,no)
if test "x$LYNX_PATH" = "xno"; then
	AC_MSG_ERROR([Calo needs the Lynx browser installed for HTML processing. Please see http://www.lynx.org])
fi

dnl -------------------------Checks for libraries.
pkg_modules="	
		pango >= 1.5.2
		gtk+-2.0 >= 2.10.0 
		glibmm-2.4 >= 2.11.3
		gtkmm-2.4 >= 2.9.0
		cairo >= 1.2.0
		libxml++-2.6
		libsoup-2.2"

PKG_CHECK_MODULES(PACKAGE,$pkg_modules)
AC_SUBST(PACKAGE_CFLAGS)
AC_SUBST(PACKAGE_LIBS)

dnl -------------------------Checks for header files.
AC_CHECK_HEADERS([string.h fcntl.h sys/file.h sys/param.h])

dnl Checks for library functions.
dnl AC_FUNC_ALLOCA

dnl internationalization macros
dnl AM_GNU_GETTEXT

AC_CONFIG_FILES([Makefile
doc/Makefile
man/Makefile
po/Makefile.in
po/Makefile
share/Makefile
share/icons/Makefile
src/Makefile
tests/Makefile])
AC_CONFIG_COMMANDS([default],[[ echo timestamp > stamp-h ]],[[]])
AC_OUTPUT

dnl configure.in ends here
