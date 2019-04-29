/*
   HareketSensoru.ino
    
    Aşağıdaki kaynak Türkcelestirilmiştir.
    
    ESP8266-based MQTT
    PIR motion sensor for HomeKit

    First Created: 14.08.2017
    Last  Updated: 15.08.2017
    Created by: Amruth Pabba aka QuickPi
*/

char* ssid = "WiFi"; //Wifi <Adı
char* password = "Sifre"; //Sifre
char* mqtt_server = "192.168.16.100"; //MQTT Sunucu IPsi
char* mqtt_name = "Haraket Sensoru"; //MQTT cihaz adı
char* mqtt_topic = "hareketsensoru"; //MQTT iletisim konusu
char* mqtt_ending = "/data"; //MQTT iletisim alt konusu
int pirPin = 0;  //PIR sensörünü bağlayacağınız GPIO’yu ayarlayın
bool lowPower = false; //Düşük güç kullanımı, daha yavaş uyarılar ama daha fazla pil istiyorsanız, doğru olarak ayarlayın.
int delayTime = 2000; //SADECE DÜŞÜK GÜÇ İÇİN - tespit edilen hareketin ne kadar süreyle aktif olması gerektiği

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
WiFiClient mainESP;
PubSubClient MQTT(mainESP);
long lastMsg = 0;
char msg[50];
int value = 0;
int pir = 1;

char* mqtt_subtopic = "/data"; //MQTT iletisim alt konusu
char* mqtt_maintopic = mqtt_topic;

void setup() {
  strcat(mqtt_maintopic, mqtt_subtopic);
  pinMode(pirPin, INPUT);
  Serial.begin(115200);
}

void loop() {
  if (!lowPower && WiFi.status() != WL_CONNECTED) startWiFi();
  Serial.println(digitalRead(pirPin));

  if ((digitalRead(pirPin)) == 0) delay(250);

  else {
    if (WiFi.status() != WL_CONNECTED) startWiFi();
    MQTT.loop();
    if (!MQTT.connected()) reconnect();
    MQTT.publish(mqtt_topic, "TRUE");
    Serial.println("Mesaj Yayınlandı: DOĞRU");
    while (digitalRead(pirPin) == 1) {
      if (!MQTT.connected()) reconnect();
      MQTT.publish(mqtt_topic, "TRUE");
      Serial.println("Mesaj Yayınlandı: DOĞRU");
      delay(500);
    }
    pir = 0;
    if (!MQTT.connected()) reconnect();
    if (lowPower) delay(delayTime);
    MQTT.publish(mqtt_topic, "FALSE");
    Serial.println("Mesaj Yayınlandı: YANLIŞ");
    MQTT.loop();
    delay(500);
  }
  if (lowPower) {
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    WiFi.forceSleepBegin();
    delay(1);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mesaj geldi [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  while (!MQTT.connected()) {
    Serial.print("MQTT bağlantısı deneniyor ...");
    if (MQTT.connect(mqtt_name)) {
      Serial.println("Baglandi");
    } else {
      Serial.print("Hata, rc=");
      Serial.print(MQTT.state());
      Serial.println("5 sn sonra tekrar deneyin.");
      for (int i = 0; i < 5000; i++) {
        delay(1);
      }
    }
  }
}

void startWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Baglanti Hatasi! Yeniden Baslatiliyor...");
    delay(1000);
    ESP.restart();
  }
  WiFi.hostname(mqtt_name);
  Serial.println("Hazır");
  Serial.print("IP adres: ");
  Serial.println(WiFi.localIP());
  MQTT.setServer(mqtt_server, 1883);
  MQTT.setCallback(callback);
}
