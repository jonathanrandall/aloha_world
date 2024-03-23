
#ifndef webserver_stuff_
#define webserver_stuff_

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "wheel_encoder_stuff.h"
#include "motor_drive_stuff.h"
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
IPAddress local_IP(192, 168, 1, 212);
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

    server.on("/set_encoders", HTTP_GET, handleRequest);

    server.begin();
}

void handleRequest(AsyncWebServerRequest *request)
{    
    ///get_encoders
    long tmp1, tmp2;
    tmp1 = millis();
    /// set_encoders?var=variable&val=10

    String variable = request->arg("var");
    String valValue = request->arg("val");

    // Parse the string into two integers
    long arg1, arg2;

    
    long enc[2];
   
    sscanf(valValue.c_str(), "%d_%d", &enc[0], &enc[1]);
    
    
    reset_wheel_encoder_data();
    
    robot_move(enc);



    String resp;

    resp = "message recieved";

    request->send(200, "text/plain", resp);

    
}

#endif