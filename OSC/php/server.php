<?php

$ip = 'localhost';
$port = 10000;

// create a socket
$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
if (!$socket)
        die('Error Unable to create AF_INET socket');

$r = socket_bind($socket, $ip, $port);
if (!$r)
        die('Error, Unable to bind socket');

// loop until user interrupt
while (true) {
    echo "whileloop";
    // this is what gets each datagram
    if (socket_recvfrom($socket, $b, 1024, MSG_DONTWAIT, $f, $p)) {

    }
}

?>