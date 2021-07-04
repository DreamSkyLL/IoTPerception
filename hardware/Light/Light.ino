#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Network
#define SERVER_IP "47.104.141.250:9321"
#define STASSID "EasyToInput"
#define STAPSK  "BTDreamSky"

// PORT
#define LIGHT 2

void init_serial(){
  Serial.begin(9600);
  Serial.println();
}

void init_wifi(){
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void init_light(){
  pinMode(LIGHT, OUTPUT);
}

void setup()
{
  init_serial();
  init_wifi();
  init_light();
}

void loop()
{
  if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClient client;
    HTTPClient http;

    if (http.begin(client, "http://" SERVER_IP "/light/")) {
      int httpCode = http.GET();
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
          if (payload == "1")
            digitalWrite(LIGHT, HIGH);
          else
            digitalWrite(LIGHT, LOW);
        }
      }
      http.end();
    }
  }

    
  delay(1000);
}
