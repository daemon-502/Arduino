// Put here needed librairies for WiFi and MQTT management
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// WiFi parameters
#define WLAN_SSID "iPhone Sleyter92k"
#define WLAN_PASS "12345678"

// Adafruit IO
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "daemon_501"
#define AIO_KEY "aio_ipxP99LJliJFhYiYbQ8BOEx3pygC"

int ledPin = 5; //D1 = GPIO5

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server
// and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish Attendance = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/RFID_test");

char ID;

/* Setup code */
void setup() {
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    Serial.begin(9600);
    delay(1000);

    // Connect to WiFi access point.
    Serial.println();
    delay(10);
    Serial.print("Connexion à ");
    Serial.println(WLAN_SSID);
    WiFi.begin(WLAN_SSID, WLAN_PASS);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    digitalWrite(ledPin, HIGH);
    Serial.println();
    Serial.println("Wi-Fi Connecté Succès !");
    Serial.print("NodeMCU IP Address : ");
    Serial.println(WiFi.localIP());

    // Connect to Adafruit IO
    connect();
}

// Connect to Adafruit IO via MQTT
void connect() {
    Serial.println("Connecting to Adafruit IO... ");
    int8_t ret;

    while ((ret = mqtt.connect()) != 0) {
        switch (ret) {
            case 1: Serial.println("Wrong protocol"); break;
            case 2: Serial.println("ID rejected"); break;
            case 3: Serial.println("Server unavail"); break;
            case 4: Serial.println("Bad user/pass"); break;
            case 5: Serial.println("Not authed"); break;
            case 6: Serial.println("Failed to subscribe"); break;
            default: Serial.println("Connection failed"); break;
        }

        if (ret >= 0) {
            mqtt.disconnect();
        }

        Serial.println("Retrying connection...");
        delay(5000);
    }

    Serial.println("Adafruit IO Connected!");
}

void loop() {
    // Ping Adafruit IO a few times to make sure we remain connected
    if (!mqtt.ping(3)) {
        // Reconnect to Adafruit IO
        if (!mqtt.connected()) {
            connect();
        }
    }

    if (Serial.available()) { // Update and send only after 1 seconds
        char a = Serial.read();
        ID = a;

        if (!Attendance.publish(ID)) { // Publish to Adafruit
            Serial.println("Failed");
        } else {
            Serial.println("Sent!");
        }
    }
}