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

const string c_a = "LDTTRTTRTTURRTLDTTULTLTTTS";
const string c_b = "DTTLTLTTRTRTRTRTLTULTTTS";
const string c_c = "LTTRTTRRDTTLTTLTTUTS";
const string c_d = "TTLTTRRDTTRTTRTRTTURTLTS";
const string c_e = "LTRDTUTLTLDTTLTTLTTUTS";
const string c_f = "LTRDTUTLTLDTTLTTULTTTS";
const string c_g = "LTTRTTRRDTTLTTLTTLTLTULTLTTS";
const string c_h = "LDTTURTTDRTTRRTLTTULTLTTTS";
const string c_i = "DTTULTTDLTTRRTRTTULTTS";
const string c_j = "LTRRDTLTTLTTULLTTLTS";
const string c_k = "LDTTURTDTRRTLTRTRRTTRTULTS";
const string c_L = "LTTRRDTTLTTUTS";
const string c_m = "LDTTRTRTRRTRTRTTULTS";
const string c_n = "LDTTRTTRTTULTS";
const string c_o = "DTTLTTLTTLTTULTTTS";
const string c_p = "LTRDTTLTLTTLTTULTTTS";
const string c_q = "TTLDTTLTTLTLTTURTLTS";
const string c_r = "LDTTRTRTRTRRTTRTULTS";
const string c_s = "DTTLTLTTRTRTTURTTLTS";
const string c_t = "LTTRDTTRRTLTTULTTS";
const string c_u = "TTLDTTULTTLDTTLTTUTS";
const string c_v = "DTLTTRTURRTTLDTTULTTTS";
const string c_w = "LTTRRDTTLTLTRRTLTLTTURRTTLTS";
const string c_x = "LTRDTTULTLTLDTTULTTS";
const string c_y = "LTRDTTLTULTTLDTLTRTULTTS";
const string c_z = "LTTRDTTRTRTTLTLTTUTS";

const string c_0 = "TLDTTLTLTTLTUTS";
const string c_1 = "LTTRDTTULTS";
const string c_2 = "LTTRDTRTRTLTLTUTS";
const string c_3 = "DTLTLTURTRDTRTTULTS";
const string c_4 = "TLDTTULTLDTLTURTLTS";
const string c_5 = "DTLTLTRTRTURTTLTS";
const string c_6 = "LTTRRDTTLTLTLTULTLTTS";
const string c_7 = "TLLRDTRTTULTS";
const string c_8 = "LTRDTRTRTRTTRTRTUTLTS";
const string c_9 = "TLDTTLTLTLTURTLTS";

const string c_non_print = "DTTUTS";

string character_commands[] = { c_0, c_1, c_2, c_3, c_4, c_5, c_6, c_7, c_8, c_9, 
                                c_a, c_b, c_c, c_d, c_e, c_f, c_g, c_h, c_i, c_j, c_k, c_l, c_m,
                                c_n, c_o, c_p, c_q, c_r, c_s, c_t, c_u, c_v, c_w, c_x, c_y, c_z };



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

void tick(){
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

void turn_left(){
  Serial.println("left");
  analogWrite(5, 1023);
  analogWrite(4, 1023);
  digitalWrite(0, LOW);
  digitalWrite(2, HIGH);
  orientLeft();
  delay(500);
  stop();
  
}
void turn_right(){
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
    //TODO ask for reset? from user
  }
}

void pen_up(){
//TODO this
}
void pen_down(){
//TODO this
}

String get_character_command(char character) {
    if (character >= '0' && character <= '9') { 
        return characters[character - '0'];
    }
    else {
        char uppercase = character & 0xdf;
        if (uppercase >= 'A' && uppercase <= 'Z') {
            return characters[10 + uppercase - 'A'];
        } else {
            return c_non_print;
        }
    }
}


void draw_character(char character) {
    String command = get_character_command(character);
    for (int = 0; i < command.length(); ++i) {
        switch(command[i]) {
            case 'L': turn_left(); break;
            case 'R': turn_right(); break;
            case 'T': tick(); break;
            case 'U': pen_up(); break;
            case 'D': pen_down(); break;
	    case 'S': transition(); break;
        }
    }
}

void draw_string(const string& message) {
   for (int i = 0; i < message.length(); ++i) {
        draw_character(message[i]);
    }
}

void drawMessage() {
  message = getRequest("/tracerracer/car.php");
  String newMessage = message.substring(message.length()-1);
  //TODO grab last line, not just last char
  Serial.println("begin##########################################");
  Serial.println(message);
  Serial.println("just the char I want");
  Serial.println(newMessage);
  Serial.println("end##########################################");
  draw_string(newMessage);  
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
