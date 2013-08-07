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
  | Author:                                                              |
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
	int data;
} dll_node;

/* list */
typedef struct _dll_list {
	struct _dll_node *head;
	struct _dll_node *tail;
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

dll_node *dll_node_ctor(int);
void dll_node_dtor(dll_node *);
void dll_node_print(dll_node *);

dll_list *dll_list_ctor();
void dll_list_dtor(dll_list *);

/*
static void *dll_list_pop(dll_list * TSRMLS_DC);
static void *dll_list_shift(dll_list *);
static void dll_list_push(dll_list *, int);
void dll_list_unshift(dll_list *, char *);
char *dll_list_top(dll_list *);
char *dll_list_bottom(dll_list *);
int dll_list_isEmpty(dll_list *);
int dll_list_count(dll_list *);
dll_node *dll_list_search(dll_list *, char *);
char *dll_list_delete(dll_list *, char *);
*/

PHP_MINIT_FUNCTION(mydllist);
PHP_MSHUTDOWN_FUNCTION(mydllist);
PHP_RINIT_FUNCTION(mydllist);
PHP_RSHUTDOWN_FUNCTION(mydllist);
PHP_MINFO_FUNCTION(mydllist);

PHP_FUNCTION(confirm_mydllist_compiled);	/* For testing, remove later. */

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
