/***************************************************
  This is an example for our Adafruit FONA Cellular Module

  Designed specifically to work with the Adafruit FONA
  ----> http://www.adafruit.com/products/1946
  ----> http://www.adafruit.com/products/1963
  ----> http://www.adafruit.com/products/2468
  ----> http://www.adafruit.com/products/2542

  These cellular modules use TTL Serial to communicate, 2 pins are
  required to interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

/*
THIS CODE IS STILL IN PROGRESS!

Open up the serial console on the Arduino at 115200 baud to interact with FONA

Note that if you need to set a GPRS APN, username, and password scroll down to
the commented section below at the end of the setup() function.
*/
#include "Adafruit_FONA.h"

#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4
#define WATERTEMP    avg3(A0)

//Change this for each well
const int well = 1;
const int reading_interval_sec = 3;
const int intervals_per_post = 3;


String http_POST_data;


int avg3(int in)
{
int avg = analogRead(in);
    avg = avg + analogRead(in);
    avg = avg + analogRead(in);
    return(avg/3);
}

//Some potential methods of getting temp in celcius

//Get temp as a float
float tempfloat(int t)
{
return(t*110.0/1023.0);
}

//Get temp as a string
String tempstr(int t)
{
long int centigrade;
int degrees, tenths;
centigrade = t*1100L/1023L;
degrees = (int) centigrade/10L;
tenths = (int) centigrade % 10L;

String degstr = String(degrees);
String tenthsstr = String(tenths);
String per = ".";

return(degstr + per + tenthsstr);
}

void gettemp() {

}


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

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

uint8_t type;

// Initialize a counter
int cnt = 0;

void setup() {
  // For temp sensor
  analogReference(INTERNAL);

  while (!Serial);

  Serial.begin(115200);
  Serial.println(F("FONA test sending temp data"));
  Serial.println(F("Initializing....(May take 3 seconds)"));

  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  type = fona.type();
  Serial.println(F("FONA is OK"));
  Serial.print(F("Found "));

  // TURN ON GPRS
  if (!fona.enableGPRS(true))
    Serial.println(F("Failed to turn on"));

  // ENABLE NTP TIME SYNC
  if (!fona.enableNTPTimeSync(true, F("pool.ntp.org")))
    Serial.println(F("Failed to enable"));
  
  // Optionally configure HTTP gets to follow redirects over SSL.
  // Default is not to follow SSL redirects, however if you uncomment
  // the following line then redirects over SSL will be followed.
  //fona.setHTTPSRedirect(true);
}


// Use this function and comment out other loop to stop operation of Arduino

//void loop() {
//  delay(3600000);
//  Serial.println("Another hour has passed");
//  }

  

void loop() {
  if (cnt == 0) {
  http_POST_data = F("{\"well\":1,\"reading_interval_s\":");
  http_POST_data += reading_interval_sec;
    // GET BATTERY DATA
  // read the ADC
  uint16_t adc;
  if (! fona.getADCVoltage(&adc)) {
   Serial.println(F("Failed to read ADC"));
  } else {
   Serial.print(F("ADC = ")); Serial.print(adc); Serial.println(F(" mV"));
   http_POST_data += F(",\"adc_voltage_mv\":");
   http_POST_data += adc;
   Serial.println(http_POST_data);
  }
  
  // read the battery voltage and percentage
  uint16_t vbat;
  if (! fona.getBattVoltage(&vbat)) {
   Serial.println(F("Failed to read Batt"));
  } else {
   Serial.print(F("VBat = ")); Serial.print(vbat); Serial.println(F(" mV"));
   http_POST_data += F(",\"batt_voltage_mv\":");
   http_POST_data += vbat;
   Serial.println(http_POST_data);
  }
  
  
  if (! fona.getBattPercent(&vbat)) {
   Serial.println(F("Failed to read Batt"));
  } else {
   Serial.print(F("VPct = ")); Serial.print(vbat); Serial.println(F("%"));
   http_POST_data += F(",\"batt_percent_charged\":");
   http_POST_data += vbat;
   Serial.println(http_POST_data);
  }
  

  // READ THE TIME
  char start_buffer[23];
  fona.getTime(start_buffer, 23);  // make sure replybuffer is at least 23 bytes!
  String start_time = start_buffer;
  start_time = start_time.substring(1, start_time.length() - 4);
  start_time.replace("/","-");
  start_time.replace(",","T");
  start_time = "20" + start_time;
  Serial.print(F("Start Time = ")); Serial.println(start_time);
  http_POST_data += F(",\"start_time\":\"");
  http_POST_data += start_time;
  http_POST_data += F("\"");

  http_POST_data += F(",\"temp_readings_c\":\"");
  
    
    }
  
  delay(1000 * reading_interval_sec);
  cnt++;
  Serial.println(cnt);
  // Check temp, add it to object
  http_POST_data += tempstr(WATERTEMP);
  
  if(cnt < intervals_per_post) {
      http_POST_data += F("|");
    }  
  Serial.println(http_POST_data);
        // If intervals_per_post time intervals have passed, post the data and reset the counter
        if(cnt >= intervals_per_post) {
            //RECORD THE END TIME
            char end_buffer[23];
            fona.getTime(end_buffer, 23);  // make sure replybuffer is at least 23 bytes!
            String end_time = end_buffer;
            end_time = end_time.substring(1, end_time.length() - 4);
            end_time.replace("/","-");
            end_time.replace(",","T");
            end_time = "20" + end_time;
            Serial.print(F("End Time = ")); Serial.println(end_time);
            http_POST_data += F("\",\"end_time\":\"");
//            http_POST_data += end_time;
            http_POST_data += F("2016-09-19T17:57:49");
            http_POST_data += F("\"}");
            delay(1000);
            
            // SEND REQUEST TO SITE
          
//            http_POST_data += "\"}";
            Serial.println("DATA TO POST");
            Serial.println(http_POST_data);
            uint16_t statuscode;
            int16_t length;
//            char url[80] = "wellcom-staging.herokuapp.com/api/device_data/";
//            char url[80] = "wellcom-staging.herokuapp.com/api/test/";
            char url[80] = "wellcom-staging.herokuapp.com/api/device_output/";
            char charBuf[500];
            http_POST_data.toCharArray(charBuf, 500);

//            char data[80] = "{\"text\":\"testdata\"}"; // TODO: THis will be information gathered from temperature data.

            // flushSerial();
            // Serial.println(F("NOTE: in beta! Use simple websites to post!"));
            // Serial.println(F("URL to post (e.g. httpbin.org/post):"));
            // Serial.print(F("http://")); readline(url, 79);
            // Serial.println(url);
            // Serial.println(F("Data to post (e.g. \"foo\" or \"{\"simple\":\"json\"}\"):"));
            // readline(data, 79);
            // Serial.println(data);

            Serial.println(F("****"));
            if (!fona.HTTP_POST_start(url, F("application/json"), (char *) charBuf, strlen(charBuf), &statuscode, (uint16_t *)&length)) {
              Serial.println("Failed!");
//              break;
            }
            while (length > 0) {
              while (fona.available()) {
                char c = fona.read();

            #if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
                loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
                UDR0 = c;
            #else
                Serial.write(c);
            #endif

                length--;
                if (! length) break;
              }
            }
            Serial.println(F("\n****"));
            fona.HTTP_POST_end();
            cnt = 0;
        }
    }

//   // flush input
//   flushSerial();
//   while (fona.available()) {
//     Serial.write(fona.read());
//   }
//
// }

void flushSerial() {
  while (Serial.available())
    Serial.read();
}

char readBlocking() {
  while (!Serial.available());
  return Serial.read();
}




//MIGHT BE ABLE TO REMOVE THIS LATER
uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout) {
  uint16_t buffidx = 0;
  boolean timeoutvalid = true;
  if (timeout == 0) timeoutvalid = false;

  while (true) {
    if (buffidx > maxbuff) {
      //Serial.println(F("SPACE"));
      break;
    }

    while (Serial.available()) {
      char c =  Serial.read();

      //Serial.print(c, HEX); Serial.print("#"); Serial.println(c);

      if (c == '\r') continue;
      if (c == 0xA) {
        if (buffidx == 0)   // the first 0x0A is ignored
          continue;

        timeout = 0;         // the second 0x0A is the end of the line
        timeoutvalid = true;
        break;
      }
      buff[buffidx] = c;
      buffidx++;
    }

    if (timeoutvalid && timeout == 0) {
      //Serial.println(F("TIMEOUT"));
      break;
    }
    delay(1);
  }
  buff[buffidx] = 0;  // null term
  return buffidx;
}

