#include <SPI.h>
#include <LoRa.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

// Define HSPI pins for LoRa
#define LORA_NSS  15
#define LORA_SCK  14
#define LORA_MISO 12
#define LORA_MOSI 13
#define LORA_RST  4
#define LORA_DIO0 2

void setup() {
    Serial.begin(115200);
    SerialBT.begin("ESP32_BT");  // Start Bluetooth first
    delay(1000);  // Allow Bluetooth to initialize

    Serial.println("Bluetooth started. Waiting for connections...");

    // Initialize HSPI for LoRa
    SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_NSS);
    LoRa.setPins(LORA_NSS, LORA_RST, LORA_DIO0);

    if (!LoRa.begin(433E6)) {
        Serial.println("LoRa init failed!");
        while (1);
    }

    Serial.println("LoRa initialized.");
}

void loop() {
    // Check for messages from Bluetooth
    if (SerialBT.available()) {
        String btMessage = SerialBT.readString();
        Serial.println("Received from Bluetooth: " + btMessage);

        // Send message via LoRa
        LoRa.beginPacket();
        LoRa.print(btMessage);
        LoRa.endPacket();

        Serial.println("Sent via LoRa: " + btMessage);
    }

    // Check for incoming LoRa messages
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        String receivedMessage = "";
        while (LoRa.available()) {
            receivedMessage += (char)LoRa.read();
        }

        Serial.println("Received via LoRa: " + receivedMessage);

        // Send received message back to Bluetooth
        SerialBT.println(receivedMessage);
    }
}
