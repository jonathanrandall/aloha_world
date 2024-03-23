
#ifndef webserver_stuff_
#define webserver_stuff_

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "wheel_encoder_stuff.h"
//wifi credentials are in here
// #include "wifi_p.h"
#include "/home/jonny/Documents/old_laptop/Arduino/Projects/ssid_stuff.h"

// Replace with your network credentials
//I've included mine in the wifi_p.h file
// const char *ssid = "";
// const char *password = "";

#define AUTO_STOP_INTERVAL 2000
unsigned long lastMotorCommand;

/// Added static IP
// Set your Static IP address
IPAddress local_IP(192, 168, 1, 211);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   // optional
IPAddress secondaryDNS(8, 8, 4, 4); // optional

AsyncWebServer server(80);
void handleRequest(AsyncWebServerRequest *request);

void init_wifi()
{
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
    {
        Serial.println("STA Failed to configure");
    }
    WiFi.mode(WIFI_AP_STA); // need this mode to use esp now

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");

    Serial.print("Go to: http://");
    Serial.println(WiFi.localIP());

    server.on("/get_encoders", HTTP_GET, handleRequest);

    server.begin();
}

void handleRequest(AsyncWebServerRequest *request)
{    
    ///get_encoders
    String resp;
    resp = String(counter1) + " " + String(counter1);

    request->send(200, "text/plain", resp);

    reset_wheel_encoder_data();
}

#endif