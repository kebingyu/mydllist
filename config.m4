dnl $Id$
dnl config.m4 for extension mydllist

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(mydllist, for mydllist support,
dnl Make sure that the comment is aligned:
dnl [  --with-mydllist             Include mydllist support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(mydllist, whether to enable mydllist support,
Make sure that the comment is aligned:
[  --enable-mydllist           Enable mydllist support])

if test "$PHP_MYDLLIST" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-mydllist -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/mydllist.h"  # you most likely want to change this
  dnl if test -r $PHP_MYDLLIST/$SEARCH_FOR; then # path given as parameter
  dnl   MYDLLIST_DIR=$PHP_MYDLLIST
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for mydllist files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       MYDLLIST_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$MYDLLIST_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the mydllist distribution])
  dnl fi

  dnl # --with-mydllist -> add include path
  dnl PHP_ADD_INCLUDE($MYDLLIST_DIR/include)

  dnl # --with-mydllist -> check for lib and symbol presence
  dnl LIBNAME=mydllist # you may want to change this
  dnl LIBSYMBOL=mydllist # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $MYDLLIST_DIR/lib, MYDLLIST_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_MYDLLISTLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong mydllist lib version or lib not found])
  dnl ],[
  dnl   -L$MYDLLIST_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(MYDLLIST_SHARED_LIBADD)

  PHP_NEW_EXTENSION(mydllist, mydllist.c, $ext_shared)
fi
