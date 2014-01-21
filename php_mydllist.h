/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: kyu                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_MYDLLIST_H
#define PHP_MYDLLIST_H

extern zend_module_entry mydllist_module_entry;
#define phpext_mydllist_ptr &mydllist_module_entry

#ifdef PHP_WIN32
#	define PHP_MYDLLIST_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_MYDLLIST_API __attribute__ ((visibility("default")))
#else
#	define PHP_MYDLLIST_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/* node */
typedef struct _dll_node {
	struct _dll_node *prev;
	struct _dll_node *next;
	void *data;
} dll_node;

/* list */
typedef struct _dll_list {
	dll_node *head;
	dll_node *tail;
	int count;
} dll_list;

/* obj */
typedef struct _dll_object {
	zend_object std;
	dll_list *llist;
	/*
	dll_node *traverse_pointer;
	int      *traverse_position;
	*/
} dll_object;

PHP_MINIT_FUNCTION(mydllist);
PHP_MSHUTDOWN_FUNCTION(mydllist);
PHP_RINIT_FUNCTION(mydllist);
PHP_RSHUTDOWN_FUNCTION(mydllist);
PHP_MINFO_FUNCTION(mydllist);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(mydllist)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(mydllist)
*/

/* In every utility function you add that needs to use variables 
   in php_mydllist_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as MYDLLIST_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define MYDLLIST_G(v) TSRMG(mydllist_globals_id, zend_mydllist_globals *, v)
#else
#define MYDLLIST_G(v) (mydllist_globals.v)
#endif

#endif	/* PHP_MYDLLIST_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
