/*==================[ file header ]============================================
 * File:    main.cpp
 * Author:  Agustin Bassi (https://github.com/agustinBassi)
 * Licence: GPLV3+
 * Version: 1.0.0
 * Date:    July 2020
====================[inclusions]=============================================*/

// Add this MACRO before to include any header
#define ARDUINOJSON_USE_LONG_LONG 1

#include <Arduino.h>
#include "Thing.h"
#include "WebThingAdapter.h"

/*==================[macros and definitions]=================================*/

// APPLICATION SETTINGS
#define LED_ONBOARD                 2
#define INIT_DELAY                  3000
#define SERIAL_BAURDATE             115200
#define WIFI_CONNECTION_DELAY       500

/*==================[internal data declaration]==============================*/

// WiFi settings
const char* WIFI_SSID = "Fibertel WiFi152 2.4GHz";
const char* WIFI_PASS = "0043510112";
// Webthings objects
WebThingAdapter* WebthingAdapter;
// Available LED types for thing
const char* LedTypes[] = {
    "OnOffSwitch", 
    "Light", 
    nullptr
    };
// the thing object description
ThingDevice LedThing(
    "led", 
    "ESP32 LED", 
    LedTypes
    );
// The property assigned to LedThing
ThingProperty LedOnProperty(
    "on", 
    "", 
    BOOLEAN, 
    "OnOffProperty"
    );

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

void Wifi_EstablishConnection() {
    // Print network SSID
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);
    // Try to connect
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    // Wait until connection is established
    while (WiFi.status() != WL_CONNECTED) {
        delay(WIFI_CONNECTION_DELAY);
        Serial.print(".");
    }
    // Report IP address
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void WebThing_CreateThings(){
    // Instance the adapter object
    WebthingAdapter = new WebThingAdapter("w25", WiFi.localIP());
    // Add properties to thing
    LedThing.addProperty(&LedOnProperty);
    // Add devices to adapter
    WebthingAdapter->addDevice(&LedThing);
    // Start the server
    WebthingAdapter->begin();
    // Print serial info about the thing
    Serial.println("Webthing HTTP server started");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.print("/things/");
    Serial.println(LedThing.id);
}

/*==================[external functions definition]==========================*/

void setup(void){
    // wait a moment before start
    delay(INIT_DELAY);
    // Configure serial port at 115200 baudrios
    Serial.begin(SERIAL_BAURDATE);
    // Configure pins of buttons and leds
    pinMode(LED_ONBOARD, OUTPUT);
    // print to console Init message
    Serial.println("Welcome to WebThings Device!");
    // connect to wifi network
    Wifi_EstablishConnection();
    // create the things that will be exposed
    WebThing_CreateThings();
    // put led in a known state
    digitalWrite(LED_ONBOARD, LOW);
}

void loop(void){
    // create a variable to save the last state
    static bool lastState = false;
    // update the state of device
    WebthingAdapter->update();
    // save the state of device
    bool state = LedOnProperty.getValue().boolean;
    // write the LED with state received (active low led)
    digitalWrite(LED_ONBOARD, state ? LOW : HIGH);
    // inform the state change
    if (state != lastState) {
        Serial.print("Changing state of thing '");
        Serial.print(LedThing.id);
        Serial.print("' to: ");
        Serial.println(state);
    }
    lastState = state;
}

/*==================[end of file]============================================*/