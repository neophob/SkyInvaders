<?php
	error_reporting(E_WARNING);
	require_once("OSC.php");

	echo "hello example\n";
    $c = new OSCClient();
    $c->set_destination("127.0.0.1", 10000);

	$m1 = new OSCMessage("/test", array("aStringParameter"));
	$m1->add_arg(31337.31337, "f");

	$c->send($m1);
?>