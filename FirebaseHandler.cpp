#include "FirebaseHandler.h"
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

FirebaseHandler::FirebaseHandler(const char* ssid, const char* password, const char* apiKey, const char* databaseUrl) 
    : ssid(ssid), password(password), apiKey(apiKey), databaseUrl(databaseUrl), signupOK(false) {}

void FirebaseHandler::setupWiFi() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.print(F("Connecting to Wi-Fi"));
    
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(300);
    }
    
    Serial.println();
    Serial.print(F("Connected with IP: "));
    Serial.println(WiFi.localIP());
}

void FirebaseHandler::setupFirebase() {
    config.api_key = apiKey;
    config.database_url = databaseUrl;
    
    if (Firebase.signUp(&config, &auth, "", "")) {
        Serial.println(F("Firebase Signup Success"));
        signupOK = true;
    } else {
        Serial.printf(F("Firebase Signup Failed: %s\n"), config.signer.signupError.message.c_str());
    }

    config.token_status_callback = tokenStatusCallback;
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
}


void FirebaseHandler::sendJsonToFirebase(String databasePath,FirebaseJson json,bool log) {    
    if (Firebase.RTDB.setJSON(&fbdo, databasePath.c_str(), &json)) {
      if(log)
        Serial.println(F("✅ JSON uploaded successfully!"));
    } else {
        Serial.print(F("❌ Error: "));
        Serial.println(fbdo.errorReason());
    }
}


 FirebaseJson FirebaseHandler::getJsonFromFirebase(String databasePath,bool log) {
    FirebaseJson json; // Create an empty FirebaseJson object
    
    if (Firebase.RTDB.getJSON(&fbdo, databasePath.c_str())) {
      if(log)
        Serial.println(F("✅ JSON retrieved successfully!"));
        json = fbdo.to<FirebaseJson>(); // Convert the response to FirebaseJson
    } else {
        Serial.print(F("❌ Error: "));
        Serial.println(fbdo.errorReason());
    }

    return json; // Return the JSON object
}
