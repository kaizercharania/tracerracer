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
    $set_file = "data/settings_$ip";
    $settings = file_get_contents($set_file);
    $fields = explode(",", $settings, 4);
    if (strcmp($fields[2], "time") == 0)
    {
        $timezone = $fields[0];
        $now = new DateTime();
        $now->setTimezone(new DateTimeZone($timezone));
        echo $now->format('H:i');
    } else if (strcmp($fields[2], "temp") == 0) {
        // wttr.in usage: https://www.maketecheasier.com/weather-details-linux-command-line/
        // shortening wttr.in output: https://github.com/chubin/wttr.in/issues/30
        // removing color shell characters: http://www.commandlinefu.com/commands/view/3584/remove-color-codes-special-characters-with-sed
        // grep returning previous chars: http://stackoverflow.com/a/8101776/2325220
        // return only some char: http://stackoverflow.com/a/538676/2325220
        system("curl -s -N wttr.in/$fields[1]?u | head -n 7 | sed \"s,\x1B\[[0-9;]*[a-zA-Z],,g\" | grep -o -P '.{3}°F' | cut -c 1-3 ");
    } else {
        echo "$fields[3]";
    }
?>
