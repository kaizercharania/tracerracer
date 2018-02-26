<?php 
    header('Content-type: text/plain'); 
    $timezone = $_GET['timezone'];
    $zipcode = $_GET['zipcode'];
    $message_type = $_GET['messagetype'];
    $message = $_GET['message'];

    // taken from here:
    // http://stackoverflow.com/a/55790/2325220
    if (!empty($_SERVER['HTTP_CLIENT_IP'])) {
        $ip = $_SERVER['HTTP_CLIENT_IP'];
    } elseif (!empty($_SERVER['HTTP_X_FORWARDED_FOR'])) {
        $ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
    } else {
        $ip = $_SERVER['REMOTE_ADDR'];
    }

    $set_file = fopen("data/settings_$ip", "w");
    fwrite($set_file, "$timezone,$zipcode,$message_type,$message");
    fclose($set_file);
    print "Recieved: $timezone, $zipcode, $message_type, $message\n";
?>

