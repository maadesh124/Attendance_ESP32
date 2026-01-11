# Embedded Attendance Monitoring System (ESP32 + Web Interface)

An **embedded attendance monitoring system** built using **ESP32**, **RFID authentication**, **keypad & LCD interface**, and **Firebase cloud backend**, with a **web-based interface** for teachers to monitor and manage attendance.

---

## 🚀 Features

- 📡 **RFID-based student identification**
- ⌨️ **Keypad input** for subject code and secure login
- 📺 **LCD interface** for user interaction
- ☁️ **Real-time attendance upload to Firebase**
- 📧 **Automatic email alerts** to parents of absentees
- 🌐 **Web interface** for teachers to view, edit, and manage attendance
- 🔐 Password-protected subject-wise attendance
- 🕒 Date-based attendance storage

---

## 🛠️ Hardware Components Used

- **ESP32**
- **MFRC522 RFID Reader**
- **4×4 Matrix Keypad**
- **LCD Display**
- RFID Cards / Tags
- WiFi Connectivity

---

## 🔌 Pin Configuration

### RFID (SPI)
| Signal | ESP32 Pin |
|--------|----------|
| SDA    | GPIO 5   |
| RST    | GPIO 22  |
| MOSI   | GPIO 23  |
| MISO   | GPIO 19  |
| SCK    | GPIO 18  |

### Keypad
- Rows: `GPIO 4, 16, 17, 2`
- Columns: `GPIO 15, 13, 21, 26`

---

## 📂 Project Structure

```
Attendance-System/
├── Attendance.ino
├── FirebaseHandler.h / .cpp
├── MailClientESP.h / .cpp
├── README.md
```

---

## 📚 Dependencies (Arduino Libraries)

### 1️⃣ Firebase Client (ESP32)
```
https://github.com/mobizt/FirebaseClient
```

### 2️⃣ ESP Mail Client
```
https://github.com/mobizt/ESP-Mail-Client
```

### 3️⃣ RFID Library (MFRC522 v2)
Search and install **MFRC522v2** by **GithubCommunity** from Arduino Library Manager.

### 4️⃣ Keypad Library
Install **Keypad** by Mark Stanley & Alexander Brevig.

---

## ⚙️ Configuration

Update the following in `Attendance.ino` before uploading:

```cpp
#define WIFI_SSID "your_wifi_name"
#define WIFI_PASSWORD "your_wifi_password"

#define API_KEY "your_firebase_api_key"
#define DATABASE_URL "your_firebase_database_url"

#define email "sender_email@gmail.com"
#define password "email_app_password"
```

> ⚠️ Use **App Passwords** for Gmail, not your actual email password.

---

## 🌐 Web Interface

Teachers can log in and view or edit attendance using the web interface:

**URL:** [Attendance Web Page](https://projt1-3abb3.web.app/html/login.html)

Features:
- View attendance by date and subject
- Mark absentees manually if needed
- Simple login with subject code and password

---

## 🧩 Firebase Database Structure

```
Subjects/
  CS01/
    password: "1234"
    name: "Embedded Systems"
    students: ["rfid1", "rfid2"]

Students/
  rfid1/
    name: "Student A"
    rollno: "21EC001"
    parentMail: "parent@email.com"

Attendance/
  CS01/
    2025-04-16/
      rfid1: true
      rfid2: false
```

---

## 🧪 How the System Works

1. Teacher enters **Subject Code** using keypad or web login.
2. Password verification from Firebase.
3. Students scan **RFID cards**.
4. Attendance marked locally on ESP32.
5. On completion (`*` key), firmware uploads attendance to Firebase.
6. Absentees trigger automatic **email alerts** to parents.
7. Teachers can view and edit attendance via the web interface.

---

## 🐞 Debugging

- Serial debugging at **115200 baud**
- Logs include:
  - RFID UID detection
  - Firebase operations
  - Attendance marking
  - Email notifications

---

## 📌 Limitations

- Uses libraries for peripherals (RFID, Keypad, LCD, Firebase)
- Web interface is minimalistic
- Network reliability depends on WiFi stability

---




## 📎 Repository

🔗 GitHub: [Attendance Web App Repo](https://github.com/maadesh124/Attendance-WebApp)

---



