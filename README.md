V projekte sú 3 kódy. 
1-	Pri otvorení brány senzor rozsvieti diodu
2-	Pri otvorení brány sa okrem diody rozbzučí bzičiak
3-	Okrem hore uvedeného sa pošle aj mailová notifikácia o otvorení brány


https://www.instructables.com/Pocket-Size-Ultrasonic-Measuring-Tool-With-ESP32/ - ultrazvukový senzor
https://uniot.sk/Navod?id=26 – posielanie emailu

https://www.youtube.com/watch?v=GNK1vzaHIZ4&ab_channel=UploadIdeasWithItamar – youtube návod.
Komentár ku kódu:

1. Zahrnuté knižnice
•	<Arduino.h>: Základné funkcie platformy Arduino.
•	<ESP_Mail_Client.h>: Knižnica pre prácu s e-mailami (SMTP).
•	<WiFi.h>: Knižnica na pripojenie k WiFi sieti.

2. Konfigurácia hardvéru
•	WiFi nastavenia:
o	ssid a password: Uchováva názov WiFi siete a heslo pre pripojenie.
•	Ultrazvukový senzor:
o	trigPin a echoPin: Piny senzora na vysielanie a prijímanie ultrazvukových signálov.
•	LED dióda a bzučiak:
o	ledPin a buzzerPin: Piny pre ovládanie LED diódy a aktívneho bzučiaka.

3. SMTP nastavenia
•	Definujú sa údaje potrebné na odosielanie e-mailov:- získate od emailového poskytovateľa v nastaveniach účtu
o	SMTP_HOST a SMTP_PORT: Server a port SMTP.
o	ODOSIELATEL_EMAIL a ODOSIELATEL_HESLO: Prihlasovacie údaje odosielateľa.
o	PRIJEMCA_EMAIL: E-mail príjemcu.

4. Funkcie
•	Overenie(SMTP_Status status):
o	Kontroluje stav odoslania e-mailu.
o	Vypíše správu o úspešnosti alebo chybe odoslania.
•	sendEmail():
o	Vytvorí správu s upozornením o otvorenej bráne a odošle ju cez SMTP server.

5. Setup funkcia- nastavenie programu (všetko čo sa v programe neopakuje)
•	setup():
o	Nastaví piny na vstupy/výstupy.
o	Pripojí ESP32 k WiFi.
o	Inicializuje SMTP nastavenia pre odosielanie e-mailov.

6. Hlavný cyklus (loop)
•	Meranie vzdialenosti:
o	Vysiela signál cez trigPin a meria odozvu cez echoPin.
o	Vypočíta vzdialenosť na základe času odozvy.
•	Podmienky pre otvorenú bránu:
o	Ak je vzdialenosť väčšia ako thresholdDistance, znamená to, že brána je otvorená.
o	Ak je brána otvorená viac ako 2 minúty, odošle e-mailové upozornenie (ak od posledného e-mailu prešlo 5 minút).
•	Resetovanie stavu:
o	Ak je brána zatvorená, vynulujú sa časy lastOpenTime a lastEmailTime.
•	LED a bzučiak:
o	Aktivujú sa, ak je brána otvorená.
o	Deaktivujú sa, ak je brána zatvorená.




7. Časové kontroly
•	millis():
o	Používa sa na sledovanie času, ktorý uplynul od zapnutia mikrokontroléra.
o	Slúži na kontrolu, či je brána otvorená dlhšie ako 2 minúty a či bol odoslaný e-mail.

8. Praktický priebeh
•	Pri zapnutí: ESP32 sa pripojí k WiFi a nastaví piny.
•	Monitorovanie: Senzor meria vzdialenosť a kontroluje stav brány.
•	Akcia:
o	Ak je brána otvorená príliš dlho, rozsvieti sa LED, zapne sa bzučiak a odošle sa e-mail.
o	Ak je brána zatvorená, všetky výstrahy sa deaktivujú.

