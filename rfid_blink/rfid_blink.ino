// This sketch turns on a specific LED based on cards detected by the MFRC522 RFID reader.

/* MFRC522 Pin Layout (from the MFRC522 library):
 * ----------------------------------------
 *             MFRC522      Arduino     
 *             Reader/PCD   Uno/101     
 * Signal      Pin          Pin         
 * ----------------------------------------
 * RST/Reset   RST          9           
 * SPI SS      SDA(SS)      10          
 * SPI MOSI    MOSI         11 / ICSP-4 
 * SPI MISO    MISO         12 / ICSP-1 
 * SPI SCK     SCK          13 / ICSP-3 
 */

#include <SPI.h>  // for serial communication between Arduino board and MFRC522
#include <MFRC522.h>  // for using MFRC522


MFRC522 mfrc522(10, 9);  // instantiate MFRC522 object

byte uidCard1[4] = {};  // known UID of first card
byte uidCard2[4] = {};  // known UID of second card
bool isCard1Set = false;  // has uid for first card been set
bool isCard2Set = false;  // has uid for second card been set
int ledPin1 = 4;  // pin of first LED
int ledPin2 = 2;  // pin of second LED

// possible cards
enum Card {CARD1, CARD2, UNKNOWN};
Card currentCard;

void setup() {
  // set up LEDs
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  
  // allow Arduino board to communicate with computer via USB
  Serial.begin(9600);

  // allow Arduino board to communicate with MFRC522
  SPI.begin();

  mfrc522.PCD_Init(); // set up RFID reader
  delay(4);  // wait 4ms to finish setup

  // check if RFID reader is connected properly
  byte versionNumber = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  Serial.println();
  if (versionNumber != 0x00 && versionNumber != 0xFF)
    Serial.println("The RFID Reader has been connected properly! \n You can scan your RFID card now...");
  else
    Serial.println("Your RFID Reader was not connected properly!");
}

void loop() {
  // Only proceed if a new card is in close proximity to the MFRC522.
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;

  // Only proceed if the UID of the card has been read.
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;

  // set uid for first and second card if not set
  uidInitializer(mfrc522.uid);

  // check if the card's UID is same as card1's
  if (mfrc522.uid.uidByte[0] == uidCard1[0] &&
      mfrc522.uid.uidByte[1] == uidCard1[1] &&
      mfrc522.uid.uidByte[2] == uidCard1[2] &&
      mfrc522.uid.uidByte[3] == uidCard1[3]
  ) currentCard = CARD1;
  
  // check if the card's UID is same as card2's
  else if (
      mfrc522.uid.uidByte[0] == uidCard2[0] &&
      mfrc522.uid.uidByte[1] == uidCard2[1] &&
      mfrc522.uid.uidByte[2] == uidCard2[2] &&
      mfrc522.uid.uidByte[3] == uidCard2[3])
  currentCard = CARD2;

  // card is neither card 1 nor card 2
  else currentCard = UNKNOWN;
  
  // turn on led1 if card is card 1
  if (currentCard == CARD1){
    digitalWrite(ledPin1, HIGH);
    delay(5000);
    digitalWrite(ledPin1, LOW);
  }
  // turn on led2 if card is card2
  else if (currentCard == CARD2){
    digitalWrite(ledPin2, HIGH);
    delay(5000);
    digitalWrite(ledPin2, LOW);
  }
  // if card is not known
  else
    Serial.println("\nCard is not recognized!");
}


// function for setting the initial uid for card 1 and card 2
void uidInitializer(MFRC522::Uid uid){
  if ( ! isCard1Set){
      uidCard1[0] = uid.uidByte[0];
      uidCard1[1] = uid.uidByte[1];
      uidCard1[2] = uid.uidByte[2];
      uidCard1[3] = uid.uidByte[3];
      isCard1Set = true;
  }
  else if (! isCard2Set){
    if ( ! (uid.uidByte[0] == uidCard1[0] && 
            uid.uidByte[1] == uidCard1[1] &&
            uid.uidByte[2] == uidCard1[2] &&
            uid.uidByte[3] == uidCard1[3]) )
         {
          uidCard2[0] = uid.uidByte[0];
          uidCard2[1] = uid.uidByte[1];
          uidCard2[2] = uid.uidByte[2];
          uidCard2[3] = uid.uidByte[3];
          isCard2Set = true;
    }
  }

}