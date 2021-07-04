#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Network
#define SERVER_IP "47.104.141.250:9321"
#define STASSID "EasyToInput"
#define STAPSK  "BTDreamSky"

// PORT
#define RAIN 4
#define WINDOW 0


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

void init_gas_fire(){
  pinMode(RAIN, INPUT);
  pinMode(WINDOW, INPUT);
}

void setup()
{
  init_serial();
  init_wifi();
  init_gas_fire();
}

void loop()
{
  bool is_rain = !(bool)digitalRead(RAIN);
  bool is_window = !(bool)digitalRead(WINDOW);

  String data = "&is_rain=" + String(is_rain) + "&is_window=" + String(is_window);

  if ((WiFi.status() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    http.begin(client, "http://" SERVER_IP "/window/");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    Serial.print("[HTTP] POST...\n");
    int httpCode = http.POST(data);

    if (httpCode > 0) {
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }

    
  delay(1000);
}
