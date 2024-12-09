// Definujeme piny pre ultrazvukový senzor
const int trigPin = 21; // Pripojený na TRIG pin ultrazvukového senzora
const int echoPin = 5; // Pripojený na ECHO pin ultrazvukového senzora

// Definujeme pin pre LED diódu
const int ledPin = 23; // Pripojený na pin, kde je zapojená červená LED dióda

// Definujeme pin pre aktívny bzučiak

const int buzzerPin = 4;  // GPIO pin, na ktorý je pripojený aktívny bzučiak

// Práh vzdialenosti na určenie, či je brána otvorená (v centimetroch)
const int thresholdDistance = 5; // prispôsob podľa potreby

void setup() {
  // Nastavenie pinov
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Inicializácia sériovej komunikácie pre monitorovanie vzdialenosti
  Serial.begin(115200);
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

  // Zobrazenie vzdialenosti na sériový monitor
  Serial.print("Vzdialenosť: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Kontrola, či je brána otvorená
  if (distance > thresholdDistance) {
    // Ak je brána otvorená, rozsvieti červenú LED diódu a zapne bzučiak
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
  } else {
    // Ak je brána zatvorená, zhasne LED diódu a vypne bzučiak
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
  }

  // Malé oneskorenie pred opakovaním
  delay(500);
}
