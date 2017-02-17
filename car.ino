#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

//////////////////////////////////////
// SET THESE TO YOUR AP CREDENTIALS //
//////////////////////////////////////
const char* ssid = "....";
const char* password = "....";

const char* host = "jeremysorensen.site";
const int httpPort = 80;

String message; // the message the car writes

ESP8266WebServer server(httpPort);

String getRequest(String url) {
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return "";
  } 
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return "";
    }
  }

  String response;
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
    response += line;
  }
  return response;
}

////////////////////////////////
// TODO implement writing !!! //
////////////////////////////////
void drawMessage() {
  message = getRequest("/tracerracer/car.php");
  Serial.println("##########################################");
  Serial.println(message);
  Serial.println("##########################################");
}

///////////////////////////////////////
// TODO implement motion sensing !!! //
///////////////////////////////////////
bool sensedSomething() {
  return false;
}

void initCar() {
  IPAddress address = WiFi.localIP();
  String url = "/tracerracer/car_init.php?ip=";
  url += address[0];
  url += ".";
  url += address[1];
  url += ".";
  url += address[2];
  url += ".";
  url += address[3];
  getRequest(url);
}

void connectWifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
}

void handleError(int code, String message){
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(code, "text/plain", message);
}

void handleRoot() {
  handleError(403, "Forbidden\n\n");
}

void handleNotFound() {
  handleError(404, "File Not Found\n\n");
}

void connectServer() {
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.on("/start", drawMessage);

  server.begin();
  Serial.println("HTTP server started");  
}

void setup(void){
  Serial.begin(115200);
  connectWifi();
  initCar();
  connectServer();
}

void loop(void){
  if (sensedSomething()) { drawMessage(); }
  server.handleClient();
}
