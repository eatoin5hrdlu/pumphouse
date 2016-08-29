# pumphouse

Arduino software for a device to measure water flow by logging the air temperature, the temperature in the stream of water, and the conductivity between two stainless steel rods in the stream of water.

CONDUCTIVITY (Analog input A4):

Two stainless steel rods, one grounded and the other connected through a 10M ohm resistor to 5v. The rod connected to the 10M ohm pullup ia also connected to A4 analog input of the Arduino. 

When no water is flowing, the conductivity between the stainless steel rods is very high and the analog reading is the full scale value 1023.  When water flows over the rods, the value drops to something lower, depending upon the ionic content of the water.


AIR TEMPERATURE (Analog input A2):

One LM35DZ is postioned just behind the  faucet connection and is hanging down (to be in the shade) measuring the ambient (AIR) temperature. It has a rubber tubing surround to avoid direct thermal contact with the faucet hardware.

This temperature sensor is connected to Analog input A2 on the Arduino.

WATER TEMPERATURE (Analog input A0):

One LM35DZ temperature sensor is next to the stainless steel rods and therefore is positioned where the water stream will pass over it. 

This temperature sensor is connected to Analog input A0 on the Arduino

The Arduino reads each analog input three times and averages the result.
The logging software reads these averages three times and prints them.
(Sorry everything is in threes, that's confusing).

Every number you see is an average of three readings, but then I print out three of those (averaged) readings, to show that there is some variability even after that first level of averaging.  Conductivity is very stable when the rods are dray, since we are pulling the stainless steel rod high with 10M ohms, but the Analog range is between 0 and 1.1 volts. A 10 M pullup in conjunction with a 4.7M ohm pulldown resistor might increase sensitivity by using more of the analog to digital converter input range.

Note the following in the Arduino output below:

The readings are at 1-second intervals.

I touched the conductivity rods with a wet Q-Tip twice which caused the
conductivity reading and the water temperature to drop momentarily.

WATER: 25.4C 25.4C 25.8C  AIR: 25.0C 25.1C 25.0C   CONDUCTIVITY: 1023 1023 1023


WATER: 25.8C 25.6C 25.4C  AIR: 25.1C 25.2C 25.2C   CONDUCTIVITY: 1023 1023 1023


WATER: 25.2C 25.8C 25.6C  AIR: 25.0C 25.1C 25.1C   CONDUCTIVITY: 1023 1023 1023


WATER: 25.5C 25.4C 25.4C  AIR: 25.1C 25.1C 25.1C   CONDUCTIVITY: 851 895 894


WATER: 25.8C 25.8C 25.6C  AIR: 25.3C 25.3C 25.0C   CONDUCTIVITY: 690 659 656


WATER: 25.5C 25.3C 25.1C  AIR: 25.1C 25.3C 25.2C   CONDUCTIVITY: 1023 1023 1023


WATER: 24.6C 24.5C 24.6C  AIR: 25.2C 25.2C 25.2C   CONDUCTIVITY: 1023 1023 1023


WATER: 24.5C 24.4C 24.7C  AIR: 25.3C 25.2C 25.1C   CONDUCTIVITY: 1023 1023 1023


WATER: 24.5C 24.8C 24.9C  AIR: 25.3C 25.3C 25.3C   CONDUCTIVITY: 1023 1023 1023


WATER: 24.6C 24.5C 24.6C  AIR: 25.1C 25.1C 25.2C   CONDUCTIVITY: 1023 1023 1023


WATER: 24.6C 24.5C 24.4C  AIR: 25.2C 25.2C 25.2C   CONDUCTIVITY: 387 387 388


WATER: 24.1C 24.1C 24.6C  AIR: 25.1C 25.4C 25.1C   CONDUCTIVITY: 492 490 492


WATER: 23.7C 24.1C 23.9C  AIR: 25.2C 25.2C 25.3C   CONDUCTIVITY: 1023 1023 1023


