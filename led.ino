#include <LiquidCrystal_I2C.h>

// Définition des broches
#define RED_PIN   5
#define GREEN_PIN 6
#define BLUE_PIN  7

int compteur = 0;

// Ecran LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);

    // Ecran LCD
    if (!i2CAddrTest(0x27)) {
        lcd = LiquidCrystal_I2C(0x3F, 16, 2);
    }
    lcd.init();
    lcd.backlight(); // Allumer
    lcd.print("Hello World xd"); // Message du dessus
}

// ATTENTION CONTRE-INTUITIF : 
// LOW allume la LED (courant passe)
// HIGH éteint la LED (pas de courant)

// LED en rouge
void turnOnRed() {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, HIGH);  
}

// LED en bleu
void turnOnBlue() {
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, LOW);
}

// Test I2C
bool i2CAddrTest(uint8_t addr) {
    Wire.begin();
    Wire.beginTransmission(addr);
    return Wire.endTransmission() == 0;
}

// Fonction RGB & LCD GYROPHARE
void loop() {
    // Afficher le compteur sur l'écran LCD
    lcd.setCursor(0, 1);
    lcd.print("COMPTEUR: ");
    lcd.print(compteur);

    // GYROPHARE
    turnOnRed();
    delay(500); 
    compteur++;

    lcd.setCursor(0, 1);
    lcd.print("COMPTEUR: ");
    lcd.print(compteur);

    turnOnBlue();
    delay(250);
    compteur++; // INCREMENTATION
}