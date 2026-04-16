#include <Wire.h>
#include <Adafruit_PN532.h>

#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

uint8_t uid_autorizado[] = {0x2A, 0xF2, 0x15, 0x07};

void setup(void) {
  Serial.begin(115200);
  Serial.println("Iniciando PN532...");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("No se encontró el PN532");
    while (1);
  }

  Serial.println("PN532 encontrado");
  nfc.SAMConfig();

  Serial.println("Esperando tarjeta...");
}

void loop(void) {
  uint8_t success;
  uint8_t uid[7];
  uint8_t uidLength;

  success = nfc.readPassiveTargetID(
    PN532_MIFARE_ISO14443A,
    uid,
    &uidLength
  );

  if (success) {

    Serial.println("Tarjeta detectada:");

    Serial.print("HEX: ");
    for (int i = 0; i < uidLength; i++) {
      if (uid[i] < 0x10) Serial.print("0"); // formato bonito
      Serial.print(uid[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    Serial.print("DEC: ");
    for (int i = 0; i < uidLength; i++) {
      Serial.print(uid[i], DEC);
      Serial.print(" ");
    }
    Serial.println();

    Serial.print("BIN: ");
    for (int i = 0; i < uidLength; i++) {
      for (int bit = 7; bit >= 0; bit--) {
        Serial.print((uid[i] >> bit) & 1);
      }
      Serial.print(" ");
    }
    Serial.println();

    // -------- VALIDACIÓN --------
    bool acceso = true;

    if (uidLength != 4) {
      acceso = false;
    } else {
      for (int i = 0; i < 4; i++) {
        if (uid[i] != uid_autorizado[i]) {
          acceso = false;
          break;
        }
      }
    }

    if (acceso) {
      Serial.println("ACCESO PERMITIDO");
    } else {
      Serial.println("ACCESO DENEGADO");
    }

    Serial.println("");
    delay(1500);
  }
}