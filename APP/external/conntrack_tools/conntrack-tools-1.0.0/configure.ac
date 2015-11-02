AC_INIT(conntrack-tools, 1.0.0, pablo@netfilter.org)

AC_CANONICAL_HOST

AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall foreign subdir-objects dist-bzip2 1.6])

AC_PROG_CC
AC_DISABLE_STATIC
AM_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_LN_S
AM_PROG_LEX
AC_PROG_YACC

case "$host" in
*-*-linux*) ;;
*) AC_MSG_ERROR([Linux only, dude!]);;
esac

dnl Dependencies
if test -z "$ac_cv_prog_YACC"
then
	echo "*** Error: No suitable bison/yacc found. ***"
	echo "    Please install the 'bison' package."
	exit 1
fi
if test -z "$ac_cv_prog_LEX"
then
	echo "*** Error: No suitable flex/lex found. ***"
	echo "    Please install the 'flex' package."
	exit 1
fi

AC_MSG_CHECKING(flex version)
flex_version=`$ac_cv_prog_LEX --version | sed 's/version//g' | awk '/flex/ {print $2}'`
flex_major=`echo $flex_version| cut -d . -f 1`
flex_minor=`echo $flex_version| cut -d . -f 2`
flex_rev=`echo $flex_version| cut -d . -f 3`
 
if test "$flex_major" -eq "2" && test "$flex_minor" -eq "5" && test "$flex_rev" -ge "33"; then
	AC_MSG_RESULT([$flex_version. OK])
else
	AC_MSG_WARN([flex version $flex_version found.
	Version 2.5.33 or greater is required. You may experience problems
	while compilating the conntrack-tools. Please, consider to upgrade 
	flex.])
fi

PKG_CHECK_MODULES([LIBNFNETLINK], [libnfnetlink >= 1.0.0])
PKG_CHECK_MODULES([LIBNETFILTER_CONNTRACK], [libnetfilter_conntrack >= 0.9.1])

AC_CHECK_HEADERS([linux/capability.h],, [AC_MSG_ERROR([Cannot find linux/capabibility.h])])

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lc':
dnl AC_CHECK_LIB([c], [main])
# FIXME: Replace `main' with a function in `-ldl':

AC_CHECK_HEADERS(arpa/inet.h)
dnl check for inet_pton
AC_CHECK_FUNCS(inet_pton)
dnl Some systems have it, but not IPv6
if test "$ac_cv_func_inet_pton" = "yes" ; then
AC_MSG_CHECKING(if inet_pton supports IPv6)
AC_RUN_IFELSE([AC_LANG_SOURCE([[
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
int main()
  {
     struct in6_addr addr6;
     if (inet_pton(AF_INET6, "::1", &addr6) < 1)
        exit(1);
     else
        exit(0);
  }
  ]])],[ AC_MSG_RESULT(yes)
       AC_DEFINE_UNQUOTED(HAVE_INET_PTON_IPV6, 1, [Define to 1 if inet_pton supports IPv6.])
  ],[AC_MSG_RESULT(no)],[AC_MSG_RESULT(no)])
fi

# Checks for header files.
dnl AC_HEADER_STDC
dnl AC_CHECK_HEADERS([netinet/in.h stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
dnl AC_C_CONST
dnl AC_C_INLINE

# Checks for library functions.
dnl AC_FUNC_MALLOC
dnl AC_FUNC_VPRINTF
dnl AC_CHECK_FUNCS([memset])

dnl AC_CONFIG_FILES([Makefile
dnl                  debug/Makefile
dnl                  debug/src/Makefile
dnl                  extensions/Makefile
dnl                  src/Makefile])

AC_CONFIG_FILES([Makefile src/Makefile include/Makefile extensions/Makefile])
AC_OUTPUT
