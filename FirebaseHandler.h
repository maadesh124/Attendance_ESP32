#ifndef FIREBASE_HANDLER_H
#define FIREBASE_HANDLER_H
#pragma once
#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

class FirebaseHandler {
private:
    const char* ssid;
    const char* password;
    const char* apiKey;
    const char* databaseUrl;
    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig config;
    bool signupOK;

public:
    FirebaseHandler(const char* ssid, const char* password, const char* apiKey, const char* databaseUrl);
    
    void setupWiFi();
    void setupFirebase();
    void sendJsonToFirebase(String databasePath,FirebaseJson json,bool log);
    FirebaseJson getJsonFromFirebase(String databasePath,bool log);
};

#endif
