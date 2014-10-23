/*
 * Dump and beep block 0 of a MIFARE RFID card using a RFID-RC522 reader
 * Uses MFRC522 - Library to use ARDUINO RFID MODULE KIT 13.56 MHZ WITH TAGS SPI W AND R BY COOQROBOT. 
 ----------------------------------------------------------------------------- 
 * Pin layout should be as follows:
 * Signal     Pin              Pin               Pin
 *            Arduino Uno      Arduino Mega      MFRC522 board
 * ------------------------------------------------------------
 * Beeper     8                ?                 ---
 * Reset      9                5                 RST
 * SPI SS     10               53                SDA
 * SPI MOSI   11               52                MOSI
 * SPI MISO   12               51                MISO
 * SPI SCK    13               50                SCK
 *
 * Hardware required:
 * Arduino
 * PCD (Proximity Coupling Device): NXP MFRC522 Contactless Reader IC
 * PICC (Proximity Integrated Circuit Card): A card or tag using the ISO 14443A interface, eg Mifare or NTAG203.
 * The reader can be found on eBay for around 5 dollars. Search for "mf-rc522" on ebay.com. 
 */

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10    //Arduino Uno
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);        // Create MFRC522 instance.

void setup() {
        Serial.begin(9600);        // Initialize serial communications with the PC
        SPI.begin();                // Init SPI bus
        mfrc522.PCD_Init();        // Init MFRC522 card
        Serial.println("Print block 0 of a MIFARE PICC ");
}

void beepByte(int uidByte)
{
        int mapped = map(uidByte, 0, 255, 100, 1500);
        int duration = random(50, 150);
        tone(8, mapped, duration);
        delay(duration+random(10)); //Apply some swing
}

void mario()
{
        tone(8,330,150);delay(150);
        tone(8,330,150);delay(300);
        tone(8,330,150);delay(300);
        tone(8,262,150);delay(150);
        tone(8,330,200);delay(300);
        tone(8,392,200);delay(600);
        tone(8,196,200);delay(600);
}

void beepError()
{  
     for (byte i = 0; i < 3; i++)
     {
        tone(8, 35, 170);
        delay(185);
     }
     delay(200);
}

void loop() {
        
        // Look for new cards
        if ( ! mfrc522.PICC_IsNewCardPresent()) {
                return;
        }

        // Select one of the cards
        if ( ! mfrc522.PICC_ReadCardSerial())    return;
        
        // Read UID. This can be done without a key
        Serial.print("Card UID: ");    
        for (byte i = 0; i < mfrc522.uid.size; i++) {
          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(mfrc522.uid.uidByte[i], HEX);
          beepByte(mfrc522.uid.uidByte[i]);
        }
       

       
        
        //Read Sector 0. Need to use default key to read from Sectors
        // Prepare key - all sectors keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
        MFRC522::MIFARE_Key key;
        for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
        
        byte buffer[18];  
        byte block  = 0;
        byte status;
        Serial.println();
        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
        delay(200);
        if (status != MFRC522::STATUS_OK) {
           Serial.print("PCD_Authenticate() failed: ");
           Serial.println(mfrc522.GetStatusCodeName(status));
           beepError();
           mfrc522.PICC_HaltA(); // Halt PICC
           mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
           return;
        }
        
        // Read block
	byte byteCount = sizeof(buffer);
	status = mfrc522.MIFARE_Read(block, buffer, &byteCount);
	if (status != MFRC522::STATUS_OK) {
	    Serial.print("MIFARE_Read() failed: ");
	    Serial.println(mfrc522.GetStatusCodeName(status));
            beepError();
	}
        else
        { 
          Serial.print("Data: ");
	  for (byte index = mfrc522.uid.size; index < 16; index++) {
	      Serial.print(buffer[index] < 0x10 ? " 0" : " ");
	      Serial.print(buffer[index], HEX);
              beepByte(buffer[index]);
	      if ((index % 4) == 3) Serial.print(" ");
  	  }
        }
        Serial.println(" ");
        mfrc522.PICC_HaltA(); // Halt PICC
        mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
       
}
