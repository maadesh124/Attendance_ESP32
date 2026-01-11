#ifndef MAIL_CLIENT_ESP_H
#define MAIL_CLIENT_ESP_H



#include <ESP_Mail_Client.h>

class MailClientESP {
  private:
    String senderEmail;
    String senderPassword;
    static SMTPSession smtp;

    static void smtpCallback(SMTP_Status status);

  public:
    MailClientESP(const String& email, const String& password);
    void setupMailClient();
    void sendEmail(const String& recipientEmail, const String& message);
};

#endif
