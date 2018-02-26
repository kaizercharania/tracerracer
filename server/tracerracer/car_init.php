<?php
    header("Content-type: text/plain");
    $local_ip = $_GET['ip'];
    // taken from here:
    // http://stackoverflow.com/a/55790/2325220
    if (!empty($_SERVER['HTTP_CLIENT_IP'])) {
        $ip = $_SERVER['HTTP_CLIENT_IP'];
    } elseif (!empty($_SERVER['HTTP_X_FORWARDED_FOR'])) {
        $ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
    } else {
        $ip = $_SERVER['REMOTE_ADDR'];
    }
    $car_file = fopen("data/car_$ip", "w");
    fwrite($car_file, $local_ip);
    fclose($car_file);
    print "IP address recorded\n"
?>
