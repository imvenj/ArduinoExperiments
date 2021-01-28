#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiMulti.h>
#include <WebServer.h>

WiFiMulti WiFiMulti;

WebServer server(80);

// Ports
const char PORT_RELAY1 = 13;
const char PORT_RELAY2 = 12;
const char PORT_RELAY3 = 14;
const char PORT_RELAY4 = 27;
const char PORT_RELAY5 = 26;
const char PORT_RELAY6 = 25;

// Constants
const char RELAY_ON = 0;  // Relay on
const char RELAY_OFF = 1; // Relay off

void setupPinMode() {
  // Sets all ports as OUTPUT
  pinMode(PORT_RELAY1, OUTPUT);
  pinMode(PORT_RELAY2, OUTPUT);
  pinMode(PORT_RELAY3, OUTPUT);
  pinMode(PORT_RELAY4, OUTPUT);
  pinMode(PORT_RELAY5, OUTPUT);
  pinMode(PORT_RELAY6, OUTPUT);

  // Sets default state as OFF
  digitalWrite(PORT_RELAY1, RELAY_OFF);
  digitalWrite(PORT_RELAY2, RELAY_OFF);
  digitalWrite(PORT_RELAY3, RELAY_OFF);
  digitalWrite(PORT_RELAY4, RELAY_OFF);
  digitalWrite(PORT_RELAY5, RELAY_OFF);
  digitalWrite(PORT_RELAY6, RELAY_OFF);
}


void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(LED_BUILTIN, 0);
}

void handleNotFound() {
  digitalWrite(LED_BUILTIN, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(LED_BUILTIN, 0);
}

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);
  
  Serial.begin(115200);
  delay(10);
  setupPinMode();

  // We start by connecting to a WiFi network
  WiFiMulti.addAP("ACFun_2.4G", "fuckgfww");

  Serial.println();
  Serial.println();
  Serial.print("Waiting for WiFi... ");

  while(WiFiMulti.run() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  
  server.on("/relay", []() {
    String op = server.arg("op");
    String pin = server.arg("pin");
    Serial.print("Op: ");
    Serial.print(op);
    Serial.print(" pin: ");
    Serial.print(pin);
    int pinId = atoi(pin.c_str());
    Serial.print(" pinId: ");
    Serial.println(pinId);
    if (op == "off") {
      switch (pinId) {
        case 0:
          Serial.println("off 0");
          digitalWrite(PORT_RELAY1, RELAY_OFF);
          break;
        case 1:
          digitalWrite(PORT_RELAY2, RELAY_OFF);
          break;
        case 2:
          digitalWrite(PORT_RELAY3, RELAY_OFF);
          break;
        case 3:
          digitalWrite(PORT_RELAY4, RELAY_OFF);
          break;
        case 4:
          digitalWrite(PORT_RELAY5, RELAY_OFF);
          break;
        case 5:
          digitalWrite(PORT_RELAY6, RELAY_OFF);
          break;
        default:
          server.send(200, "text/plain", "pin not available");
          return;
      }
      server.send(200, "text/plain", "success");
    }
    else if (op == "on") {
      switch (pinId) {
        case 0:
          Serial.println("on 0");
          digitalWrite(PORT_RELAY1, RELAY_ON);
          break;
        case 1:
          digitalWrite(PORT_RELAY2, RELAY_ON);
          break;
        case 2:
          digitalWrite(PORT_RELAY3, RELAY_ON);
          break;
        case 3:
          digitalWrite(PORT_RELAY4, RELAY_ON);
          break;
        case 4:
          digitalWrite(PORT_RELAY5, RELAY_ON);
          break;
        case 5:
          digitalWrite(PORT_RELAY6, RELAY_ON);
          break;
        default:
          server.send(200, "text/plain", "pin not available");
          return;
      }
      server.send(200, "text/plain", "on success");
    }
    else {
      server.send(200, "text/plain", "operation not supported");
    }
  });


  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
