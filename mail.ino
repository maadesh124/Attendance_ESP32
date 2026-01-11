#include "FirebaseHandler.h"
#include "MailClientESP.h"

#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>


#define SDA 5 
#define SCK 18
#define MOSI 23
#define MISO 19
#define RST 22

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {4, 16, 17, 2};
byte colPins[COLS] = {15, 13, 21, 26};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
MFRC522 mfrc522(SDA, RST);


String date = "2025-04-16";//use the same format
#define WIFI_SSID "maadesh"
#define WIFI_PASSWORD "12345679"
// #define API_KEY "AIzaSyBUxs4hd9qvc-gX-TK0pXRhdgH6m9-HTB4"
// #define DATABASE_URL "https://test1-c569c-default-rtdb.firebaseio.com"
#define API_KEY "AIzaSyCDiKaQcdHAOp7qTGiZtp8RYbYF1o9yqI8"
#define DATABASE_URL "https://projt1-3abb3-default-rtdb.firebaseio.com/"
#define email "uditlovesgupta@gmail.com"
#define password "thsc bmhx rmel ujuc"


FirebaseHandler firebase(WIFI_SSID, WIFI_PASSWORD, API_KEY, DATABASE_URL);
MailClientESP mailClient(email,password );


String subCode;
String pwd;
String pwdEnt;
bool login = false;
FirebaseJson ids;
FirebaseJson sub;
FirebaseJson studs;
FirebaseJsonArray validIds;
FirebaseJsonData item; //dummy temp variable



String getKeypadInput(int length) {
    String input = "";
    while (input.length() < length) {
        char key = keypad.getKey();
        if (key) {
            input += key;
            Serial.print(key);
        }
    }
    Serial.println();
    return input;
}

String readRFID() {
    Serial.println("Waiting for RFID card...");
    while (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
        delay(100);
    }

    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        uid += String(mfrc522.uid.uidByte[i], HEX);
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();

    Serial.print("Card UID: ");
    Serial.println(uid);

    return uid;
}



void fetchStudents(FirebaseJson &json) {
  ids.clear();
    
    FirebaseJsonData jsonData;
    if (json.get(jsonData, "students") ) {
        validIds.setJsonArrayData(jsonData.stringValue); 
        
        Serial.println(F("Fetching Students..."));
        for (size_t i = 0; i < validIds.size(); i++) {
            
            validIds.get(item, i);  

            if (item.success) {
                //Serial.println(item.stringValue);
                String rfid=item.stringValue;
                ids.set(rfid,false);  // Print each student name
                FirebaseJson st = firebase.getJsonFromFirebase("Students/" +rfid+"/",false);
                studs.set(rfid,st);
            }
        }
    } else {
        //Serial.println("Failed to get 'students' array.");
    }
}




void verify() {
    while (!login) {
        Serial.println(F("Enter Subject Code:"));
        subCode = getKeypadInput(4);

        sub = firebase.getJsonFromFirebase("Subjects/" + subCode,false);

        sub.get(item, "password");

        if (item.success) {
            pwd = item.to<String>();
            Serial.print(F("Enter Password: "));
            pwdEnt = getKeypadInput(4);

            if (pwd.equals(pwdEnt)) {
                login = true;
                Serial.println(F("✅ Login Successful!"));
            } else {
                Serial.println(F("❌ Incorrect Password. Try Again."));
            }
        } else {
            Serial.println(F("❌ Invalid Subject Code. Try Again."));
        }

        fetchStudents(sub);
    }
}

String getStudent(String rfid){

studs.get(item,rfid+"/"+"name");
String name=item.to<String>();
studs.get(item,rfid+"/"+"rollno");
String rollno=item.to<String>();

return name+":"+rollno;
}

void finish() {
    //FirebaseJson data;
    
    firebase.sendJsonToFirebase("Attendance/" + subCode+"/"+date, ids,true);
    Serial.println(F("✅ Attendance Uploaded!"));
      String jsonStr;
  ids.toString(jsonStr, true); // true = prettified, false = compact
  Serial.println(jsonStr);
  processAbsentees();
    login = false;
}


void processAbsentees() {

        sub.get(item, "name");
        String subName=item.to<String>();
        Serial.println(F("Absentees..."));
        for (size_t i = 0; i < validIds.size(); i++) {
            
            validIds.get(item, i);  
            String rfid=item.stringValue;
            ids.get(item, rfid);
            String att=item.to<String>();
            Serial.println(att);
            if(att=="false"){
              studs.get(item, rfid+"/name");
              String name=item.to<String>();
              Serial.println(name);
              studs.get(item,rfid+"/parentMail");
              String parentMail=item.to<String>();
             // Serial.println(parentMail+"=p mail");

              mailClient.sendEmail(parentMail, "Hello! This message is from XYZ.\n"+name + " was absent on " + date + " in class " + subName + ".");


            }
            
        }
   
}

void setup() {
    Serial.begin(115200);
    SPI.begin();
    mfrc522.PCD_Init();

    firebase.setupWiFi();
    firebase.setupFirebase();
    mailClient.setupMailClient();

    verify(); 
}



void loop() {
    if (login) {
        Serial.println(F("Scan RFID or Press '*' to finish:"));

        while (true) {
            
            String id = readRFID(); 
            Serial.println(getStudent(id));
            if(ids.get(item,id))
            ids.set(id,true);
            else
            Serial.println(F("❌ Student not Found"));
            String ch=getKeypadInput(1);

            if (ch.equals("*")) {
                finish();
                
                break;
            }
        }
    }
    else
    verify();
}




