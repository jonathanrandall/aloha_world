
#ifndef webserver_stuff_
#define webserver_stuff_

//for the wifi get stuff, I've followed the tutorial from Random Nerds
// https://randomnerdtutorials.com/esp32-client-server-wi-fi/

#include <WiFi.h>
#include <HTTPClient.h>
#include <ESPAsyncWebServer.h>
#include "wheel_encoder_stuff.h"
#include "motor_drive_stuff.h"
//wifi credentials are in here
// #include "wifi_p.h"
const char* serverEncoder = "http://192.168.1.211/get_encoders";

// Replace with your network credentials
//I've included mine in the ssid_stuff.h file

#include "/home/jonny/Documents/old_laptop/Arduino/Projects/ssid_stuff.h"
// const char *ssid = "";
// const char *password = "";

#define AUTO_STOP_INTERVAL 2000
unsigned long lastMotorCommand;

/// Added static IP
// Set your Static IP address
IPAddress local_IP(192, 168, 1, 182);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   // optional
IPAddress secondaryDNS(8, 8, 4, 4); // optional

AsyncWebServer server(80);
void handleRequest(AsyncWebServerRequest *request);

void init_wifi()
{
  Serial.println("in init wifi");
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
    {
        Serial.println("STA Failed to configure");
    }
    Serial.println("in init wifi 2");
    WiFi.mode(WIFI_AP_STA); // need this mode to use esp now
    Serial.println("in init wifi 3");

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

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
    Serial.println(payload);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

void http_get_loop(void *params){
  String enc_vals;
  long cnt_all[3];
  TickType_t xLastWakeTime;
  const TickType_t xFrequency = pdMS_TO_TICKS(750); // 500ms period
  
  while (true){
    vTaskDelayUntil( &xLastWakeTime, xFrequency );
    
    enc_vals = httpGETRequest(serverEncoder);
    sscanf(enc_vals.c_str(), "%d %d %d", &cnt_all[0], &cnt_all[1], &cnt_all[2]);
    if(cnt_all[0]<4) cnt_all[0]=0; //get rid of noise
    if(cnt_all[1]<4) cnt_all[0]=0; //get rid of noise
    Serial.print("counter 1: ");
    Serial.print(cnt_all[0]);
    Serial.print(", counter 2: ");
    Serial.println(cnt_all[2]);
    xQueueSend(cmd_queue, &cnt_all, pdMS_TO_TICKS(400));//portMAX_DELAY);
    

  }
}





#endif