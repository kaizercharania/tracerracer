# server
the server files

## Quick Start:
1. put tracerracer folder on server under html folder.
2. make sure data folder (under tracerracer folder) is owned by the same user as the server (for me this is www-data), that way the PHP scripts can add files to it.
3. Change the ssid and password in car.ino for your access point
4. Upload car.ino and open the serial monitor
5. Go to http://[HOST]/tracerracer and change some information
7. Click update settings_
8. Click start car, and look at the output of the serial monitor.

## What the WiFi Module Does
1. Connects to wifi (gets assigned a local, private ip address)
2. Sets up a server (not an access point)
3. Sends does an http request to http://[HOST]/tracerracer/car_init.php?id=[PRIVATE_IP_ADDRESS]
4. Whenever a request is made inside the LAN to [PRIVATE_IP_ADDRESS]/start :
  1. sends a request to http://[HOST]/tracerracer/car.php
  2. Gets the text response
  3. Draws the response (by printing to the serial port for now)
    
## What the Server Does
1. Serves up tracerracer/index.html
2. When it gets a request like http://[HOST]/tracerracer/car_init.php?id=[PRIVATE_IP_ADDRESS] :
  1. Looks up the IP address of the client (this is the public ip address of the access point, which is assumed to be the same for the car and the client)
  2. Saves the private IP address to data/car_[PUBLIC_IP_ADDRESS]
3. When it gets a request like http://[HOST]/tracerracer/settings.php?timezone=[TIMEZONE]&zipcode=[ZIPCODE]&messagetype=[time|temp|custom]&message=[CUSTOM_MESSAGE] :
  1. Looks up the IP address of the client (this is the public ip address of the access point, which is assumed to be the same for the car and the client)
  2. Saves all the parameters as a comma delimited string to data/settings_[PUBLIC_IP_ADDRESS]
4. When it gets a request like http://[HOST]/tracerracer/car.php :
  1. Looks up the public IP
  2. Opens data/settings_[PUBLIC_IP_ADDRESS]
  3. Responds with the appropriate text, time (based on timezone), temp (based on zip code) or custom text
5. When it gets a request like http://[HOST]/tracerracer/start.php :
  1. Looks up the public IP
  2. Opens data/car_[PUBLIC_IP_ADDRESS]
  3. Responds with the private local IP address of the ESP2866

## What the Client does
1. User sets time zone, zip code, message type (time, temp or message) and custom message
2. When user clicks "Update Settings" button, sends a request to http://[HOST]/tracerracer/settings.php?timezone=[TIMEZONE]&zipcode=[ZIPCODE]&messagetype=[time|temp|custom]&message=[CUSTOM_MESSAGE]
3. When the user clicks "Start Car" :
  1. sends a request to http://[HOST]/tracerracer/start.php
  2. Gets the response, which is the private, local IP address of the ESP2866
  3. Posts a request to [PRIVATE_IP_ADDRESS]/start
