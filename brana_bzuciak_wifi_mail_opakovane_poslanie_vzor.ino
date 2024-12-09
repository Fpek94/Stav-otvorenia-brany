#include <Arduino.h> // Zahrnie všetky knižnice dostupné v IDE
#include <ESP_Mail_Client.h> // Zahrnie knižnicu na prácu s e-mailom
#include <WiFi.h> // Načítanie WiFi knižnice pre ESP32

// WiFi nastavenia
const char* ssid = "xxxxx";      // Názov WiFi siete (SSID)
const char* password = "xxxxxx";  // Heslo k WiFi

// Definujeme piny pre ultrazvukový senzor
const int trigPin = 21; // Pripojený na TRIG pin ultrazvukového senzora
const int echoPin = 5;  // Pripojený na ECHO pin ultrazvukového senzora

// Definujeme pin pre LED diódu
const int ledPin = 23; // Pripojený na pin, kde je zapojená červená LED dióda

// Definujeme pin pre aktívny bzučiak
const int buzzerPin = 4;  // GPIO pin, na ktorý je pripojený aktívny bzučiak

// Práh vzdialenosti na určenie, či je brána otvorená (v centimetroch)
const int thresholdDistance = 5; // prispôsob podľa potreby

// SMTP nastavenia
#define SMTP_HOST "smtp.azet.sk" // Definuje premennú SMTP_HOST
#define SMTP_PORT 465 // Definuje SMTP_PORT (predvolený port serveru)

// Definujeme e-mail odosielateľa a príjemcu
#define ODOSIELATEL_EMAIL "espbrana@azet.sk" // E-mail odosielateľa
#define ODOSIELATEL_HESLO "ESP32Test." // Heslo e-mailu odosielateľa
#define PRIJEMCA_EMAIL "xxxxxxx" // E-mail príjemcu

SMTPSession smtp; // Objekt na odosielanie e-mailov
ESP_Mail_Session udaje; // Objekt pre konfiguráciu e-mailu
SMTP_Message sprava; // Objekt pre správu, ktorú posielame

unsigned long lastOpenTime = 0;  // Uchováva čas otvorenia brány
unsigned long lastEmailTime = 0; // Uchováva čas posledného odoslania e-mailu

void Overenie(SMTP_Status status) {
  if (status.success()) {
    Serial.println("E-mail úspešne odoslaný.");
  } else {
    Serial.println("Chyba pri odosielaní e-mailu:");
    Serial.println(status.info());
  }
}

void sendEmail() {
  sprava.sender.name = "ESP32";
  sprava.sender.email = ODOSIELATEL_EMAIL;
  sprava.subject = "ESP32 Overovaci email";
  sprava.addRecipient("Majiteľ", PRIJEMCA_EMAIL);
  String text_spravy = "Brána je otvorená viac ako 2 minúty - Poslane z ESP32";
  sprava.text.content = text_spravy.c_str();

  // Pripojenie k SMTP serveru a odoslanie e-mailu
  if (!smtp.connect(&udaje)) {
    Serial.println("Chyba pri pripojení k SMTP serveru.");
    return;
  }

  if (!MailClient.sendMail(&smtp, &sprava)) {
    Serial.println("Chyba pri odosielaní e-mailu: " + smtp.errorReason());
  }
}

void setup() {
  // Nastavenie pinov
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Inicializácia sériovej komunikácie pre monitorovanie vzdialenosti
  Serial.begin(115200);
  delay(100);

  // WiFi pripojenie
  Serial.println("Pripajam sa na WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi pripojenie úspešné!");
  Serial.print("IP adresa: ");
  Serial.println(WiFi.localIP());

  // Nastavenie SMTP pripojenia
  udaje.server.host_name = SMTP_HOST;
  udaje.server.port = SMTP_PORT;
  udaje.login.email = ODOSIELATEL_EMAIL;
  udaje.login.password = ODOSIELATEL_HESLO;
  udaje.login.user_domain = "";

  smtp.callback(Overenie); // Funkcia na overenie stavu odosielania
}

void loop() {
  // Poslanie signálu do ultrazvukového senzora
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Výpočet vzdialenosti na základe času odozvy
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;

  // Ak je brána otvorená
  if (distance > thresholdDistance) {
    if (lastOpenTime == 0) {
      lastOpenTime = millis();  // Nastavíme čas otvorenia
      lastEmailTime = lastOpenTime; // Nastavíme čas prvého e-mailu
    } else {
      // Ak je brána otvorená viac ako 2 minúty a ešte sme neposlali e-mail
      if (millis() - lastOpenTime >= 120000 && millis() - lastEmailTime >= 300000) {
        sendEmail();
        lastEmailTime = millis(); // Aktualizujeme čas posledného e-mailu
      }
    }

    // Zobrazenie času otvorenia brány v sekundách
    Serial.print("Čas otvorenia brány: ");
    Serial.print((millis() - lastOpenTime) / 1000);  // Čas otvorenia v sekundách
    Serial.println(" sekúnd");

    // Rozsvietenie LED a aktivovanie bzučiaka
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
  } else {
    // Ak je brána zatvorená, resetujeme čas otvorenia a čas posledného e-mailu
    lastOpenTime = 0;          // Resetovanie času
    lastEmailTime = 0;         // Resetovanie času e-mailu
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
  }

  // Malé oneskorenie pred opakovaním
  delay(500);
}
