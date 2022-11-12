#include <Arduino.h>
#include "DHTesp.h"
#include <ESP8266WiFi.h>
#include <espnow.h>

ADC_MODE(ADC_VCC);

#define dataPin D5
DHTesp dht;

uint8_t macDestinataire[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

String deviceId = "SALON";

#define MESH_ID               6734922
#define GROUP_HT              2
#define wakeUpIntervalMinutes 5

typedef struct struct_message {
  int     mesh_id;
  uint8_t sensor_id[6];
  byte    category;
  bool    status ;
  float   temperature;
  float   humidity;
  float   battery;
} struct_message;

struct_message  msg;



void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

void sendMessage()
{
  Serial.println("Envoi ON");
  esp_now_send(macDestinataire, (uint8_t *)&msg, sizeof(msg));
  Serial.println("Envoi OK");
  // tempo de 2 secondes après émission
  delay(2000);
}

void initESPNow()
{
  WiFi.mode(WIFI_STA);
  Serial.print("Adresse MAC : ");
  Serial.println(WiFi.macAddress());
  
  Serial.println("Initialisation ESP-Now");
  if (esp_now_init() != 0) {
    Serial.println("Erreur lors de l initialisation de ESP-NOW - restart");
    delay(1000);
    ESP.restart();
  } 
  Serial.println("Defintion du role controller");
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  Serial.println("Enregistrement de la callback d apres envoi");
  esp_now_register_send_cb(OnDataSent);
  Serial.println("Enregistrement destinataire");
  esp_now_add_peer(macDestinataire, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  Serial.println("ESP-NOW initialized");
}

void processMeasurement()
{
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  float heatIndex = dht.computeHeatIndex(temperature, humidity, false);
  float battery = ESP.getVcc() / 1000.00;

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity);
  Serial.print("\t\t");
  Serial.print(temperature);
  Serial.print("\t\t");
  Serial.println(heatIndex, 1);
  Serial.print("\t\t");
  Serial.println(battery, 1);

  msg.battery = battery;
  msg.category = GROUP_HT;
  msg.humidity = humidity;
  msg.mesh_id = MESH_ID;
  memcpy( msg.sensor_id, deviceId.c_str(), deviceId.length());
  msg.status = true;
  msg.temperature = temperature;
  sendMessage();
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting ESP-now temperature sensor");
  Serial.print(deviceId);
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\tHeatIndex (C)\tBattery");
  dht.setup(dataPin, DHTesp::DHT22);

  initESPNow();
  processMeasurement();
  ESP.deepSleep(wakeUpIntervalMinutes * 60e6); 
}

void loop()
{

}
