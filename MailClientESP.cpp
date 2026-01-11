#include "MailClientESP.h"

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

// ✅ Define static variable here
SMTPSession MailClientESP::smtp;

MailClientESP::MailClientESP(const String& email, const String& password)
  : senderEmail(email), senderPassword(password) {}

void MailClientESP::setupMailClient() {


  if (WiFi.status() == WL_CONNECTED) {
    Serial.print(F("Already connected to Wi-Fi with IP: "));
    Serial.println(WiFi.localIP());
  } else {
    Serial.println(F("Wi-Fi not connected!"));
  }

  MailClient.networkReconnect(true);
  MailClientESP::smtp.debug(1);                          // ✅ static access
  MailClientESP::smtp.callback(smtpCallback);            // ✅ static access
}

void MailClientESP::sendEmail(const String& recipientEmail, const String& messageText) {
  Session_Config config;
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = senderEmail.c_str();
  config.login.password = senderPassword.c_str();
  config.login.user_domain = "";
  config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  config.time.gmt_offset = 3;
  config.time.day_light_offset = 0;

  SMTP_Message message;
  message.sender.name = F("ESP");
  message.sender.email = senderEmail.c_str();
  message.subject = F("ESP Test Email");
  message.addRecipient(F("User"), recipientEmail.c_str());
  message.text.content = messageText.c_str();
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

  if (!MailClientESP::smtp.connect(&config)) {           // ✅ static access
    ESP_MAIL_PRINTF(F("Connection error, Status Code: %d, Error Code: %d, Reason: %s\n"),
                    MailClientESP::smtp.statusCode(), MailClientESP::smtp.errorCode(), MailClientESP::smtp.errorReason().c_str());
    return;
  }

  if (!MailClientESP::smtp.isLoggedIn()) {               // ✅ static access
    Serial.println(F("Login failed."));
  } else {
    Serial.println(F("Logged in successfully."));
  }

  if (!MailClient.sendMail(&MailClientESP::smtp, &message)) { // ✅ static access
    ESP_MAIL_PRINTF(F("Error sending mail, Status Code: %d, Error Code: %d, Reason: %s\n"),
                    MailClientESP::smtp.statusCode(), MailClientESP::smtp.errorCode(), MailClientESP::smtp.errorReason().c_str());
  }
}

void MailClientESP::smtpCallback(SMTP_Status status) {
  Serial.println(status.info());

  if (status.success()) {
    Serial.println("----------------");
    ESP_MAIL_PRINTF(F("Message sent success: %d\n"), status.completedCount());
    ESP_MAIL_PRINTF(F("Message sent failed: %d\n"), status.failedCount());
    Serial.println(F("----------------\n"));

    for (size_t i = 0; i < MailClientESP::smtp.sendingResult.size(); i++) { // ✅ static access
      SMTP_Result result = MailClientESP::smtp.sendingResult.getItem(i);    // ✅ static access
      Serial.printf(F("Message No: %d\n"), i + 1);
      Serial.printf(F("Status: %s\n"), result.completed ? "success" : "failed");
      Serial.printf(F("Recipient: %s\n"), result.recipients.c_str());
      Serial.printf(F("Subject: %s\n"), result.subject.c_str());
      Serial.printf(F("Timestamp: %s\n"),
                   MailClient.Time.getDateTimeString(result.timestamp, "%B %d, %Y %H:%M:%S").c_str());
    }

    MailClientESP::smtp.sendingResult.clear(); // ✅ static access
  }
}
