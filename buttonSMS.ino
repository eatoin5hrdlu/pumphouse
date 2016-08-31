#include "Adafruit_FONA.h"

#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4

// this is a large buffer for replies
char replybuffer[255];

// We default to using software serial. If you want to use hardware serial
// (because softserial isnt supported) comment out the following three lines
// and uncomment the HardwareSerial line
#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

// Hardware serial is also possible!
//  HardwareSerial *fonaSerial = &Serial1;

// Use this for FONA 800 and 808s
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);
// Use this one for FONA 3G
//Adafruit_FONA_3G fona = Adafruit_FONA_3G(FONA_RST);

uint8_t type;

// this constant won't change:
const int  buttonPin = 0;    // the pin that the pushbutton is attached to

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = HIGH;         // current state of the button
int lastButtonState = HIGH;     // previous state of the button

unsigned long lastSentMessageTimeInMilliseconds = millis();
int lastSentButtonCount = 0;
int totalMessagesSent = 0;

void setup() {
  while (!Serial){
    //Wait for Serial to exist.
  }
  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT);
  // initialize serial communication:
  Serial.begin(115200);

  Serial.println(F("FONA basic test"));
  Serial.println(F("Initializing....(May take 3 seconds)"));

  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  type = fona.type();
  Serial.println(F("FONA is OK"));
  Serial.print(F("Found "));
  switch (type) {
    case FONA800L:
      Serial.println(F("FONA 800L")); break;
    case FONA800H:
      Serial.println(F("FONA 800H")); break;
    case FONA808_V1:
      Serial.println(F("FONA 808 (v1)")); break;
    case FONA808_V2:
      Serial.println(F("FONA 808 (v2)")); break;
    case FONA3G_A:
      Serial.println(F("FONA 3G (American)")); break;
    case FONA3G_E:
      Serial.println(F("FONA 3G (European)")); break;
    default:
      Serial.println(F("???")); break;
  }

  // Print module IMEI number.
  char imei[15] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("Module IMEI: "); Serial.println(imei);
  }

  // Optionally configure a GPRS APN, username, and password.
  // You might need to do this to access your network's GPRS/data
  // network.  Contact your provider for the exact APN, username,
  // and password values.  Username and password are optional and
  // can be removed, but APN is required.
  //fona.setGPRSNetworkSettings(F("your APN"), F("your username"), F("your password"));

  // Optionally configure HTTP gets to follow redirects over SSL.
  // Default is not to follow SSL redirects, however if you uncomment
  // the following line then redirects over SSL will be followed.
  //fona.setHTTPSRedirect(true);
}

void loop() {
  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == LOW) {
      // if the current state is HIGH then the button
      // wend from off to on:
      buttonPushCounter++;
      Serial.print("on - ");
      Serial.print("number of button pushes:  ");
      Serial.println(buttonPushCounter);
    } else {
      // if the current state is LOW then the button
      // wend from on to off:
      Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    delay(250);
  }
  // save the current state as the last state,
  //for next time through the loop
  lastButtonState = buttonState;
  unsigned long currentTime = millis();
  if(currentTime - lastSentMessageTimeInMilliseconds > 10000){
    lastSentMessageTimeInMilliseconds = currentTime;
    int buttonCountsSinceLastMessage = buttonPushCounter - lastSentButtonCount;
    lastSentButtonCount = buttonPushCounter;
    if(buttonCountsSinceLastMessage>0){
      if(++totalMessagesSent <= 2){
        String message = "Total Button Count since last message: ";
        message += String(buttonCountsSinceLastMessage);
        Serial.println("Sending: " + message);
        char messageBuffer[141];
        message.toCharArray(messageBuffer, 141);
        if(fona.sendSMS("2068025842", messageBuffer)){
          Serial.println("Sent button count");
        } else {
          Serial.println("Error in SMS");
        }
      } else {
        Serial.println("Already sent 2 messages since last reboot");
      }
    } else {
      Serial.println("No recent button presses");
    }
  }
}
