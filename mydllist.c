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
  | Author: kyu                                                          |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_mydllist.h"

/* If you declare any globals in php_mydllist.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(mydllist)
*/

/* True global resources - no need for thread safety here */
static int le_mydllist;
zend_class_entry *dll_list_ce;
static zend_object_handlers dll_list_obj_handlers;

PHP_METHOD(MyDLList, __construct) {/*{{{*/
	dll_list *list = NULL;
	zval *object = getThis();

	list = dll_list_ctor();
	dll_object *obj = (dll_object *)zend_object_store_get_object(object TSRMLS_CC);
	obj->llist = list;
}/*}}}*/

PHP_METHOD(MyDLList, __destruct) {/*{{{*/
}/*}}}*/

PHP_METHOD(MyDLList, push) {/*{{{*/
	long data; /* use int will cause segment why? */

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &data) == FAILURE) {
		RETURN_NULL();
	}
	dll_object *obj = (dll_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	dll_list *list = obj->llist;

	dll_node *node = dll_node_ctor(data);
	node->prev = list->tail;
	if (list->tail) {
		list->tail->next = node;
		node->prev = list->tail;
	} else {
		list->head = node;
	}
	list->tail = node;
	list->count++;
}/*}}}*/

PHP_METHOD(MyDLList, pop) {/*{{{*/
	dll_object *obj = (dll_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	dll_list *list = (dll_list *)obj->llist;
	dll_node *tail = list->tail;
	if (tail) {
		if (tail->prev) {
			tail->prev->next = NULL;
		} else {
			list->head = NULL;
		}
		list->tail = tail->prev;
		list->count--;
		int ret = tail->data;
		dll_node_dtor(tail);
		RETVAL_LONG(ret);
	} else {
		RETVAL_FALSE;
	}
}/*}}}*/

PHP_METHOD(MyDLList, shift) {/*{{{*/
	dll_object *obj = (dll_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	dll_list *list = obj->llist;
	dll_node *head = list->head;
	if (head) {
		if (head->next) {
			head->next->prev = NULL;
		} else {
			list->tail = NULL;
		}
		list->head = head->next;
		list->count--;
		int ret = head->data;
		dll_node_dtor(head);
		RETVAL_LONG(ret);
	} else {
		RETVAL_FALSE;
	}
}
/*}}}*/

PHP_METHOD(MyDLList, unshift) {/*{{{*/
	long data;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &data) == FAILURE) {
		RETURN_NULL();
	}
	dll_object *obj = (dll_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	dll_list *list = obj->llist;
	dll_node *node = dll_node_ctor(data);
	node->prev = NULL;
	node->next = list->head;
	node->data = data;
	if (list->head) {
		list->head->prev = node;
		node->next = list->head;
	} else {
		list->tail = node;
	}
	list->head = node;
	list->count++;
}
/*}}}*/

PHP_METHOD(MyDLList, top) {/*{{{*/
	dll_object *obj = (dll_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	dll_list *list = obj->llist;
	if (list->tail) {
		RETVAL_LONG(list->tail->data);
	} else {
		RETVAL_FALSE;
	}
}
/*}}}*/

PHP_METHOD(MyDLList, bottom) {/*{{{*/
	dll_object *obj = (dll_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	dll_list *list = obj->llist;
	if (list->head) {
		RETVAL_LONG(list->head->data);
	} else {
		RETVAL_FALSE;
	}
}
/*}}}*/

PHP_METHOD(MyDLList, isEmpty) {/*{{{*/
	dll_object *obj = (dll_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	dll_list *list = obj->llist;
	if (0 == list->count) {
		RETVAL_TRUE;
	} else {
		RETVAL_FALSE;
	}
}
/*}}}*/

PHP_METHOD(MyDLList, myCount) {/*{{{*/
	dll_object *obj = (dll_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	dll_list *list = obj->llist;
	RETVAL_LONG(list->count);
}/*}}}*/

PHP_METHOD(MyDLList, print) {/*{{{*/
	dll_object *obj = (dll_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	dll_list *list = obj->llist;
	dll_node *head = list->head;
	while (head) {
		dll_node_print(head);
		head = head->next;
	}
	php_printf("\n");
}/*}}}*/

PHP_METHOD(MyDLList, isContain) {/*{{{*/
	long data, found;
	found = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &data) == FAILURE) {
		RETURN_NULL();
	}
	dll_object *obj = (dll_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	dll_list *list = obj->llist;
	dll_node *curr = list->head;
	while (curr) {
		if (curr->data == data) {
			found = 1;
			break;
		}
		curr = curr->next;
	}
	if (1 == found) {
		RETVAL_TRUE;
	} else {
		RETVAL_FALSE;
	}
}
/*}}}*/

/*
PHP_METHOD(MyDLList, delete) {
	long data;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &data) == FAILURE) {
		RETURN_NULL();
	}
	dll_object *obj = (dll_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	dll_list *list = obj->llist;
	dll_node *curr = list->head;
	while (curr) {
		if (curr->data == data) {
			if (curr == list->head) {
				return dll_list_shift(list);
			} else if (curr == list->tail) {
				return dll_list_pop(list);
			} else {
				curr->prev->next = curr->next;
				curr->next->prev = curr->prev;
				ret = curr->data;
				dll_node_dtor(curr);
				return ret;
			}
		}
		curr = curr->next;
	}
	return NULL;
}
*/

/* {{{ mydllist_functions[]
 *
 * Every user visible function must have an entry in mydllist_functions[].
 */
const zend_function_entry mydllist_functions[] = {
	PHP_ME(MyDLList, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(MyDLList, __destruct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
	PHP_ME(MyDLList, push, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(MyDLList, pop, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(MyDLList, shift, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(MyDLList, unshift, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(MyDLList, top, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(MyDLList, bottom, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(MyDLList, myCount, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(MyDLList, isEmpty, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(MyDLList, isContain, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(MyDLList, print, NULL, ZEND_ACC_PUBLIC)
	PHP_FE(confirm_mydllist_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in mydllist_functions[] */
};
/* }}} */

/* {{{ mydllist_module_entry
 */
zend_module_entry mydllist_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"mydllist",
	mydllist_functions,
	PHP_MINIT(mydllist),
	PHP_MSHUTDOWN(mydllist),
	PHP_RINIT(mydllist),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(mydllist),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(mydllist),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_MYDLLIST
ZEND_GET_MODULE(mydllist)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("mydllist.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_mydllist_globals, mydllist_globals)
    STD_PHP_INI_ENTRY("mydllist.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_mydllist_globals, mydllist_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_mydllist_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_mydllist_init_globals(zend_mydllist_globals *mydllist_globals)
{
	mydllist_globals->global_value = 0;
	mydllist_globals->global_string = NULL;
}
*/
/* }}} */

/* dllist object dtor{{{*/
void dll_list_obj_dtor(void *object TSRMLS_DC) {
	dll_object *obj = (dll_object *)object;
	dll_list_dtor(obj->llist);

	zend_hash_destroy(obj->std.properties);
	FREE_HASHTABLE(obj->std.properties);
	efree(obj);
}
/*}}}*/

/* {{{ dllist object ctor
 */
zend_object_value dll_list_obj_ctor(zend_class_entry *ce TSRMLS_DC) {
	zval *tmp;
	zend_object_value retval;

	dll_object *obj = (dll_object *)emalloc(sizeof(dll_object));
	memset(obj, 0, sizeof(dll_object));
	obj->std.ce = ce;

	ALLOC_HASHTABLE(obj->std.properties);
	zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy(obj->std.properties, &ce->properties_info,
		(copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(obj, NULL, dll_list_obj_dtor, NULL TSRMLS_CC);
	retval.handlers = &dll_list_obj_handlers;
	/* other object initialization */
	return retval;
}
/* }}} */

dll_node *dll_node_ctor(int data) {/*{{{*/
	dll_node *node = (dll_node *)emalloc(sizeof(dll_node));
	node->prev = NULL;
	node->next = NULL;
	node->data = data;
	return node;
}/*}}}*/

void dll_node_dtor(dll_node *node) {/*{{{*/
	efree(node);
}/*}}}*/

void dll_node_print(dll_node *node) {/*{{{*/
	php_printf("%i ", node->data);
}/*}}}*/

dll_list *dll_list_ctor() {/*{{{*/
	dll_list *list = (dll_list *)emalloc(sizeof(dll_list));
	list->head = NULL;
	list->tail = NULL;
	list->count = 0;
	return list;
}/*}}}*/

void dll_list_dtor(dll_list *list) {/*{{{*/
	dll_node *curr = list->head;
	while (curr) {
		dll_node *node = curr;
		curr = curr->next;
		dll_node_dtor(node);
	}
	efree(list);
}/*}}}*/


/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(mydllist)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	zend_class_entry ce;
	/* register dll list object */
	INIT_CLASS_ENTRY(ce, "MyDLList", mydllist_functions);
	dll_list_ce = zend_register_internal_class(&ce TSRMLS_CC);
	dll_list_ce->create_object = dll_list_obj_ctor;
	/* create custom handler table */
	memcpy(&dll_list_obj_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(mydllist)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(mydllist)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(mydllist)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(mydllist)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "mydllist support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_mydllist_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_mydllist_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "mydllist", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
