
#include <Arduino.h>
#include <WiFi.h>
#include <zenoh-pico.h>
#include <BluetoothSerial.h>

#if Z_FEATURE_PUBLICATION == 1
// WiFi-specific parameters
#define SSID "ssid"
#define PASS "pass"

#define CLIENT_OR_PEER 0  // 0: Client mode; 1: Peer mode
#if CLIENT_OR_PEER == 0
#define MODE "client"
#define CONNECT "udp/192.168.4.187:55671"  // If empty, it will scout, set to static listening for best reliability
#elif CLIENT_OR_PEER == 1
#define MODE "peer"
#define CONNECT "udp/224.0.0.225:7447#iface=en0"
#else
#error "Unknown Zenoh operation mode. Check CLIENT_OR_PEER value."
#endif

#define KEYEXPR "demo/example/zenoh-pico-pub"
#define VALUE "[ARDUINO]{ESP32} Publication from Zenoh-Pico!"

z_owned_publisher_t pub;
static int idx = 0;

void sleepfunc()
{
    z_put;
    Serial.println("disengaging zenoh");
    Serial.flush();
    delay(140);
    esp_deep_sleep_start();
}
void zenohpub() {
  char buf[256];
    sprintf(buf, "[%4d] %s", idx++, VALUE);
    Serial.print("Writing Data ('");
    Serial.print(KEYEXPR);
    Serial.print("': '");
    Serial.print(buf);
    Serial.println("')");

    if (z_publisher_put(z_publisher_loan(&pub), (const uint8_t *)buf, strlen(buf), NULL) < 0) {
        Serial.println("Error while publishing data");
    }

    delay(1000);
}
void setup() {
    // Initialize Serial for debug
    Serial.begin(115200);
    while (!Serial) {
        delay(1000);
    }

    // Set WiFi in STA mode and trigger attachment
    Serial.print("Connecting to WiFi...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
    }
    Serial.println("OK");

    // Initialize Zenoh Session and other parameters
    z_owned_config_t config = z_config_default();
    zp_config_insert(z_config_loan(&config), Z_CONFIG_MODE_KEY, z_string_make(MODE));
    if (strcmp(CONNECT, "") != 0) {
        zp_config_insert(z_config_loan(&config), Z_CONFIG_CONNECT_KEY, z_string_make(CONNECT));
    }

    // Open Zenoh session
    Serial.print("Opening Zenoh Session...");
    z_owned_session_t s = z_open(z_config_move(&config));
    if (!z_session_check(&s)) {
        Serial.println("Unable to open session!");
        while (5) {
            ;
        }
    }
    Serial.println("OK");

    // Start the receive and the session lease loop for zenoh-pico
    zp_start_read_task(z_session_loan(&s), NULL);
    zp_start_lease_task(z_session_loan(&s), NULL);

    // Declare Zenoh publisher
    Serial.print("Declaring publisher for ");
    Serial.print(KEYEXPR);
    Serial.println("...");
    pub = z_declare_publisher(z_session_loan(&s), z_keyexpr(KEYEXPR), NULL);
    if (!z_publisher_check(&pub)) {
        Serial.println("Unable to declare publisher for key expression!");
        while (1) {
            ;
        }
    }
    Serial.println("OK");
    Serial.println("Zenoh setup finished!");

    delay(300);
}

void loop() {
    zenohpub();
    z_put; //close the session
    delay(1000);
    sleepfunc();
}



#else
void setup() {
    Serial.println("ERROR: Zenoh pico was compiled without Z_FEATURE_PUBLICATION but this example requires it.");
    return;
}
void loop() {}
#endif