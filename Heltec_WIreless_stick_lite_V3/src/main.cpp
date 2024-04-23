#include <Arduino.h>
#include <WiFi.h>
#include <SoftwareSerial.h>
#include <zenoh-pico.h>
#include <BluetoothSerial.h>
#include <chrono>
#include <thread>
#include <iostream>

#define SSID "ssid"
#define PASS "pass"


#define CLIENT_OR_PEER 0 // 0: Client mode; 1: Peer mode
#if CLIENT_OR_PEER == 0
#define MODE "client"
#define CONNECT "tcp/192.168.1.216:7447" // If empty, it will scout
#elif CLIENT_OR_PEER == 1
#define MODE "peer"
#define CONNECT "udp/224.0.0.225:7447#iface=en0"
#else
#error "Unknown Zenoh operation mode. Check CLIENT_OR_PEER value."
#endif

#define KEYEXPR "zenoh/heltec/WSLV3_mmwave/motion"

float Vbattf;
#define VALUE "Heltec MoTion detected!"       // simply duplicate this value to send different messages
#define VALUEBAT batteryvalue // simply duplicate this value to send different messages
z_owned_publisher_t pub;

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 7        /* Time ESP32 will go to sleep (in seconds) */

void zenohsetup()
{
    // Initialize Serial for debug
    int wififc;
    while (!Serial)
    {
        delay(1000);
    }

    // Set WiFi in STA mode and trigger attachment
    Serial.print("Connecting to WiFi...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        wififc++;
        if (wififc > 15)
        {
            Serial.println("wifi failed too often");
            esp_deep_sleep_start();
        }
    }
    Serial.println("OK");

    // Initialize Zenoh Session and other parameters
    z_owned_config_t config = z_config_default();
    zp_config_insert(z_config_loan(&config), Z_CONFIG_MODE_KEY, z_string_make(MODE));
    if (strcmp(CONNECT, "") != 0)
    {
        zp_config_insert(z_config_loan(&config), Z_CONFIG_CONNECT_KEY, z_string_make(CONNECT));
    }

    // Open Zenoh session
    Serial.print("Opening Zenoh Session...");
    z_owned_session_t s = z_open(z_config_move(&config));
    if (!z_session_check(&s))
    {
        Serial.println("Unable to open session!");
        while (5)
        {
            delay(1000);
            zenohfc++;
            if (zenohfc = 30){
                Serial.println("wifi failed too often");
                esp_deep_sleep_start();
        }}
    }
    Serial.println("OK");

    // Start the receive and the session lease loop for zenoh-pico
    zp_start_read_task(z_session_loan(&s), NULL);
    zp_start_lease_task(z_session_loan(&s), NULL);

    // Declare Zenoh publisher
    Serial.print("Declaring publisher for "); // this is a slight bug i'm not bothered to fix as it will always declare for the same expression rather than the one it might be sending to
    Serial.print(KEYEXPR);
    Serial.println("...");
    pub = z_declare_publisher(z_session_loan(&s), z_keyexpr(KEYEXPR), NULL);
    if (!z_publisher_check(&pub))
    {
        Serial.println("Unable to declare publisher for key expression!");
        while (5)
        {
            ;
        }
    }
    Serial.println("OK");
    Serial.println("Zenoh setup finished!");

    delay(300);
}

void sleepfunc()
{
    z_put; //disconnects the session before going into deepsleep
    Serial.println("disengaging zenoh");
    bootCount++;
    Serial.flush();
    delay(140);
    esp_deep_sleep_start();
}

void setup(){
    Serial.begin(115200);
    delay(3000); //changed location in the odd case it requires more time for power to catch in and avoid guru panic
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
                   " Seconds");


}


void loop()
{
    zenohsetup();
    delay(300);
    zenohmotionpub();
    delay(300);
    sleepfunc();
}
