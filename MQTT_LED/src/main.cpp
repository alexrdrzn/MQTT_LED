#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <string.h>

//MQTT #defines
#define MSG_BUFFER_SIZE 500
#define QoS 1
#define KEEP_ALIVE 15

//LED #define
#define LED 16
#define MQTT_LED 2

//Broker Address
const char *MQTT_Server = "broker.hivemq.com";

//AP constants
const char *ssid = "Totalplay-CE9E";
const char *password = "FA52D025";

WiFiClient wifi_esp32;
PubSubClient client(wifi_esp32);
uint8_t state = LOW;
const char *str;
uint32_t last = 0;
uint32_t actual = 0;

void callback(char *topic, byte *payload, unsigned int length)
{
    //AUN NO IMPLEMENTADA, A LA ESPERA DE FORMATO DE DATOS FINAL
    char message[MSG_BUFFER_SIZE] = "";
    for (int i = 0; i < length; i++)
        message[i] = (char)payload[i];
    str = message;
    if (strcmp(str, "ON"))
        digitalWrite(MQTT_LED, LOW);
    else
        digitalWrite(MQTT_LED, HIGH);
}

void setup()
{
    Serial.begin(9600);
    pinMode(LED, OUTPUT);
    pinMode(MQTT_LED, OUTPUT);
    digitalWrite(LED, LOW);
    digitalWrite(MQTT_LED, HIGH);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
    }
    Serial.println("Conectado");
    client.setServer(MQTT_Server, 1883);
    client.setCallback(callback);
    client.connect("HKI");
    client.subscribe("HKI/LED", QoS);
}

void loop()
{
    client.loop();
    actual = millis();
    if ((last + 2000) < actual)
    {
        client.publish("HKI/TEMP", "HOLA");
        digitalWrite(LED, state);
        if (state == LOW)
            state = HIGH;
        else
            state = LOW;
    }
}