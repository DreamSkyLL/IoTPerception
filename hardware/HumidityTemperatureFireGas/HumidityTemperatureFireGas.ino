#include <dht11.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Network
#define SERVER_IP "47.104.141.250:9321"
#define STASSID "EasyToInput"
#define STAPSK  "BTDreamSky"

// PORT
#define DHT11PIN 2
#define FIRE 4
#define GAS 0

dht11 DHT11;

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
  pinMode(GAS, INPUT);
  pinMode(FIRE, INPUT);
}

void setup()
{
  init_serial();
  init_wifi();
  init_gas_fire();
}

void loop()
{
  DHT11.read(DHT11PIN);
  float humidity = (float)DHT11.humidity;
  float temperature = (float)DHT11.temperature;
  bool is_fire = !(bool)digitalRead(FIRE);
  bool is_gas = !(bool)digitalRead(GAS);

  String data = "humidity=" + String(humidity) + "&temperature=" + String(temperature)
    + "&is_fire=" + String(is_fire) + "&is_gas=" + String(is_gas);

  if ((WiFi.status() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    http.begin(client, "http://" SERVER_IP "/data/");
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
