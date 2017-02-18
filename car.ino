#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

//////////////////////////////////////
// SET THESE TO YOUR AP CREDENTIALS //
//////////////////////////////////////
const char* ssid = "Zoe's iPhone";
const char* password = "b95xxiywmn9l";

const char* host = "jeremysorensen.site";
const int httpPort = 80;
char orientation = 'E';

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

void stop(void)
{
    analogWrite(5, 0);
    analogWrite(4, 0);
}

void orientLeft(){
  Serial.println("before left");
  Serial.println(orientation);
  if(orientation == 'N'){
    orientation = 'W';
  }else if(orientation == 'E'){
    orientation = 'N';  
  }else if(orientation == 'S'){
    orientation = 'E';
  }else{
    orientation = 'S';
  }
  Serial.println("after left");
  Serial.println(orientation);
  
}
void orientRight(){
  Serial.println("before right");
  Serial.println(orientation);
  if(orientation == 'N'){
    orientation = 'E';
  }else if(orientation == 'E'){
    orientation = 'S';  
  }else if(orientation == 'S'){
    orientation = 'W';
  }else{
    orientation = 'N';
  }
  Serial.println("after right");
  Serial.println(orientation);
}

void oneTick(){
    Serial.println("forward");
    analogWrite(5, 1023);
    analogWrite(4, 1023);
    digitalWrite(0, HIGH);
    digitalWrite(2, HIGH);
    delay(200);
    stop();
    analogWrite(5, 1023);
    analogWrite(4, 1023);
    digitalWrite(0, HIGH);
    digitalWrite(2, HIGH);
    delay(200);
    stop();
}

void left(){
  Serial.println("left");
  analogWrite(5, 1023);
  analogWrite(4, 1023);
  digitalWrite(0, LOW);
  digitalWrite(2, HIGH);
  orientLeft();
  delay(500);
  stop();
  
}
void right(){
  Serial.println("right");
  analogWrite(5, 1023);
  analogWrite(4, 1023);
  digitalWrite(0, HIGH);
  digitalWrite(2, LOW);
  orientRight();
  delay(500);
  stop();
}

void transition(){
  //assume robot ends facing east in prev number
  if(orientation == 'E'){
    //pen up
    oneTick();
  }else{
    Serial.println("ERROR robot cannot transition");
  }
}
 
void one(void)
{
    right();
    //lower pen
    oneTick();
    oneTick();
    //raise pen
    right();
    right();
    oneTick();
    oneTick();
    right();
    transition();  
}
 
void two(void)
{
    //lower pen
    oneTick();
    right();
    oneTick();
    right();
    oneTick();
    left();
    oneTick();
    left();
    oneTick();
    //raise pen
    left();
    oneTick();
    oneTick();
    right();
    transition();
}
 
void three(void)
{
  //lower pen
  oneTick();
  right();
  oneTick();
  right();
  oneTick();
  left();
  left();
  oneTick();
  right();
  oneTick();
  right();
  oneTick();
  //raise pen
  right();
  oneTick();
  oneTick();
  right();
  oneTick();
  transition();
   
}
 
void four(void)
{
  right();
  //lower pen
  oneTick();
  left();
  oneTick();
  right();
  oneTick();
  left();
  left();
  oneTick();
  oneTick();
  right();
  transition();   
}
void drawMessage() {
  message = getRequest("/tracerracer/car.php");
  String newMessage = message.substring(message.length()-1);
  Serial.println("begin##########################################");
  Serial.println(message);
  Serial.println("just the char I want");
  Serial.println(newMessage);
  Serial.println("end##########################################");
  if(newMessage == "1"){
    one();
  }else if(newMessage == "2"){
    two();
  }else if(newMessage == "3"){
    three();
  }else if(newMessage == "4"){
    four();
  }
  Serial.println(orientation);
  
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
  
  pinMode(5, OUTPUT); // 1,2EN aka D1 pwm left
  pinMode(4, OUTPUT); // 3,4EN aka D2 pwm right
  pinMode(0, OUTPUT); // 1A,2A aka D3
  pinMode(2, OUTPUT); // 3A,4A aka D4
}

void loop(void){
  if (sensedSomething()) { drawMessage(); }
  server.handleClient();
}
