#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define SX1278 LoRa module pins
#define NSS 10
#define SCK 13
#define MOSI 11
#define MISO 12
#define RST 9
#define DI0 2

// Initialize LCD (I2C address 0x27, 20 columns x 4 rows)
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
    Serial.begin(115200);

    // Initialize LCD
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("LoRa Receiver Ready");

    // Initialize LoRa module
    SPI.begin();
    LoRa.setPins(NSS, RST, DI0);

    if (!LoRa.begin(433E6)) {  // Ensure same frequency as sender
        Serial.println("LoRa init failed. Check connections.");
        lcd.setCursor(0, 1);
        lcd.print("LoRa Init Failed!");
        while (1);
    }
    
    Serial.println("LoRa Receiver Ready.");
    lcd.setCursor(0, 1);
    lcd.print("Listening...");
}

void loop() {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        String receivedMessage = "";
        while (LoRa.available()) {
            receivedMessage += (char)LoRa.read();
        }

        // Print received message to Serial Monitor
        Serial.print("Received via LoRa: ");
        Serial.println(receivedMessage);

        // Display message on LCD
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("LoRa Message:");
        lcd.setCursor(0, 1);
        lcd.print(receivedMessage);

        // Delay to allow message to be visible before next update
        delay(2000);
    }
}
