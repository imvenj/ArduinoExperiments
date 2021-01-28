#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


//const char* ssid = "ACFun_2.4G";
//const char* password = "fuckgfww";
const char* ssid = "360WiFi-902905";
const char* password = "1122334455";

ESP8266WebServer server(80);

WiFiEventHandler gotIpEventHandler, disconnectedEventHandler;
// Ports
const char PORT_RELAY1 = 15;
const char PORT_RELAY2 = 13;
const char PORT_RELAY3 = 12;
const char PORT_RELAY4 = 14;
const char PORT_RELAY5 = 0;
const char PORT_RELAY6 = 4;

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

void setup()
{
  Serial.begin(115200);
  Serial.println();

  pinMode(LED_BUILTIN, OUTPUT);
  setupPinMode();

  gotIpEventHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP& event)
  {
    Serial.print("Station connected, IP: ");
    Serial.println(WiFi.localIP());
  });

  disconnectedEventHandler = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected& event)
  {
    Serial.println("Station disconnected");
  });

  Serial.printf("Connecting to %s ...\n", ssid);
  WiFi.begin(ssid, password);

  // Setup mdns server and web server
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/led",  []() {
    String state=server.arg("state");
    if (state == "on") digitalWrite(LED_BUILTIN, LOW);
    else if (state == "off") digitalWrite(LED_BUILTIN, HIGH);
    else { server.send(200, "text/plain", "not supported"); return; }
    server.send(200, "text/plain", "Led is now " + state);
  });

  server.on("/relay", []() {
    String op = server.arg("op");
    String pin = server.arg("pin");
    int pinId = atoi(pin.c_str());
    if (op == "off") {
      switch (pinId) {
        case 0:
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
      server.send(200, "text/plain", "success off");
    }
    else if (op == "on") {
      switch (pinId) {
        case 0:
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
      server.send(200, "text/plain", "success on");
    }
    else {
      server.send(200, "text/plain", "operation not supported");
    }
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}


void handleRoot() {
  digitalWrite(LED_BUILTIN, 1);
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

void loop()
{
  server.handleClient();
  MDNS.update();
}
