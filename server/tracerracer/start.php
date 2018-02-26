<?php
    //header("Content-type: text/plain");

    // taken from here:
    // http://stackoverflow.com/a/55790/2325220
    if (!empty($_SERVER['HTTP_CLIENT_IP'])) {
        $ip = $_SERVER['HTTP_CLIENT_IP'];
    } elseif (!empty($_SERVER['HTTP_X_FORWARDED_FOR'])) {
        $ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
    } else {
        $ip = $_SERVER['REMOTE_ADDR'];
    }
    $car_file = "data/car_$ip";
    $car_ip = file_get_contents("/var/www/html/tracerracer/data/car_$ip");
    print "http://$car_ip/start";
?>
