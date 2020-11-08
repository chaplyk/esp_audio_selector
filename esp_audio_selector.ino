#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID ""
#define STAPSK  ""
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

String status="a";

ESP8266WebServer server(80);

void handleNotFound() {
  server.send(404, "text/plain", "Not Found\n");
}

void setup(void) {
  pinMode(14, OUTPUT);    // PIN 5, A
  pinMode(12, OUTPUT);    // PIN 6, B
  pinMode(13, OUTPUT);    // PIN 7, C
  pinMode(15, OUTPUT);    // PIN 8, D
  pinMode(4, INPUT);      // PIN 2, Button
  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/a", handleA);
  server.on("/b", handleB);
  server.on("/c", handleC);
  server.on("/d", handleD);
  server.on("/status", handleStatus);
  
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  
  handleA(); 
}

void handleA() {
  server.send(200, "text/plain", "success A");  
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(15, LOW);
  digitalWrite(14, HIGH); 
  status="a";
}

void handleB() {
  server.send(200, "text/plain", "success B");  
  digitalWrite(14, LOW);
  digitalWrite(13, LOW);
  digitalWrite(15, LOW);
  digitalWrite(12, HIGH);
  status="b";
}

void handleC() {
  server.send(200, "text/plain", "success C");  
  digitalWrite(14, LOW);
  digitalWrite(12, LOW);
  digitalWrite(15, LOW);
  digitalWrite(13, HIGH);  
  status="c";
}

void handleD() {
  server.send(200, "text/plain", "success D");  
  digitalWrite(14, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(15, HIGH);  
  status="d";
}

void handleStatus() {
  server.send(200, "text/plain", status);  
}

void loop(void) {
  if (digitalRead(4) == 1){
    if (status == "a") {
      handleB();
    }
    else if (status == "b") {
      handleC();
    }
    else if (status == "c") {
      handleD();
    }
    else if (status == "d") {
      handleA();
    }
  }
  
  delay(800);
  server.handleClient();
  MDNS.update();
}
