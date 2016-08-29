/*
 * Water Pump Station Monitor
 */
#define LED 13
#define WATERTEMP    avg3(A0)
#define AIRTEMP      avg3(A2)
#define CONDUCTIVITY avg3(A4)

int avg3(int in)
{
int avg = analogRead(in);
    avg = avg + analogRead(in);
    avg = avg + analogRead(in);
    return(avg/3);
}

void checkValues()
{
	Serial.print("AIR: ");
	printTC(AIRTEMP);
	Serial.print(" ");
	printTC(AIRTEMP);
	Serial.print(" ");
	printTC(AIRTEMP);
	Serial.print("       WATER: ");
	printTC(WATERTEMP);
	Serial.print(" ");
	printTC(WATERTEMP);
	Serial.print(" ");
	printTC(WATERTEMP);
	Serial.print("        CONDUCTIVITY: ");
	Serial.print(CONDUCTIVITY);
	Serial.print(" ");
	Serial.print(CONDUCTIVITY);
	Serial.print(" ");
	Serial.println(CONDUCTIVITY);
}

void printTC(int t)
{
long int centigrade;
int degrees, tenths;
     centigrade = t*1100L/1023L;
     degrees = (int) centigrade/10L;
     tenths = (int) centigrade % 10L;
     Serial.print(degrees);
     Serial.print(".");
     Serial.print(tenths);
     Serial.print("C");
}
	
void setup()
{
	Serial.begin(9600);
	analogReference(INTERNAL);
	pinMode(LED,       OUTPUT);  digitalWrite(LED, 0);
}

int cnt = 0;
void loop()
{
	delay(1000);
	checkValues();
}
