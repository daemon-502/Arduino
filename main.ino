#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <RFID.h>

// Définition des broches
#define RED_PIN   5
#define GREEN_PIN 6
#define BLUE_PIN  7

// Ecran LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// RFID : D10=pin SDA, D9=pin RST
RFID rfid(10, 9);
unsigned char str[MAX_LEN];

// Liste des UID
String authorizedUID = "2A9002B0";   // Seul cet UID est autorisé

void setup() {
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);

    // Initialisation écran LCD
    if (!i2CAddrTest(0x27)) {
        lcd = LiquidCrystal_I2C(0x3F, 16, 2);
    }
    lcd.init();
    lcd.backlight();
    lcd.print("Pret pour RFID!");

    // Initialisation RFID
    Serial.begin(9600);
    SPI.begin();
    rfid.init();
}

void loop() {
    // Recherche de badge
    if (rfid.findCard(PICC_REQIDL, str) == MI_OK) {
        Serial.println("Carte detectee !");

        // Lecture et affichage du numéro du badge
        if (rfid.anticoll(str) == MI_OK) {
            String cardUID = "";
            Serial.print("Numero du badge : ");
            for (int i = 0; i < 4; i++) {
                Serial.print(0x0F & (str[i] >> 4), HEX);  // Partie haute de l'octet
                Serial.print(0x0F & str[i], HEX);         // Partie basse de l'octet
                cardUID += String(0x0F & (str[i] >> 4), HEX);
                cardUID += String(0x0F & str[i], HEX);
            }
            Serial.println("");
            Serial.print("UID lu : ");
            Serial.println(cardUID);

            // Vérification de l'UID : Si l'UID correspond à celui autorisé, accès autorisé, sinon accès refusé
            if (cardUID.equalsIgnoreCase(authorizedUID)) {
                accessGranted();
            } else {
                accessDenied();
            }

            // Sélection du badge et passage en sommeil
            rfid.selectTag(str);
            rfid.halt();
        }
    }
}

void accessGranted() {
    // LED verte clignote et message "Accès autorisé"
    for (int i = 0; i < 4; i++) {
        setColor(false, true, false); // Vert
        delay(250);
        setColor(true, true, true);   // Éteint
        delay(250);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Acces autorise!");
    delay(2000);
    lcd.clear();
    lcd.print("Pret pour RFID!");
}

void accessDenied() {
    // LED rouge clignote et message "Accès refusé"
    for (int i = 0; i < 4; i++) {
        setColor(true, false, false); // Rouge
        delay(250);
        setColor(false, false, false);   // Éteint (toutes les couleurs)
        delay(250);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Acces refuse!");
    delay(2000);
    lcd.clear();
    lcd.print("Pret pour RFID!");
}

void setColor(bool red, bool green, bool blue) {
    digitalWrite(RED_PIN, red ? LOW : HIGH);
    digitalWrite(GREEN_PIN, green ? LOW : HIGH);
    digitalWrite(BLUE_PIN, blue ? LOW : HIGH);
}

// Test I2C
bool i2CAddrTest(uint8_t addr) {
    Wire.begin();
    Wire.beginTransmission(addr);
    return Wire.endTransmission() == 0;
}