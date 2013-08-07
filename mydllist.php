<?php
/*
$br = (php_sapi_name() == "cli")? "":"<br>";

if(!extension_loaded('mydllist')) {
	dl('mydllist.' . PHP_SHLIB_SUFFIX);
}
$module = 'mydllist';
$functions = get_extension_funcs($module);
echo "Functions available in the test extension:$br\n";
foreach($functions as $func) {
    echo $func."$br\n";
}
echo "$br\n";
$function = 'confirm_' . $module . '_compiled';
if (extension_loaded($module)) {
	$str = $function($module);
} else {
	$str = "Module $module is not compiled into PHP";
}
echo "$str\n";

$class = new ReflectionClass('mydllist');
$extension = $class->getExtension();
var_dump($extension->getClassNames());
 */

$list = new MyDLList();
$list->push(12);
$list->unshift(13);
echo $list->top() . "\n";
echo $list->bottom() . "\n";
var_dump($list->isContain(12));
$list->print();
$s = $list->shift();
var_dump($s);
$list->print();
$n = $list->myCount();
var_dump($n);
$list->pop();
var_dump($list->isContain(12));
$list->pop();
unset($list);
