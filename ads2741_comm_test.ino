/* 
Code to read the Texas Instruments / Burr Brown ADS7841 ADC
12-bit Analog to Digital Converter, 4 Channels Single Ended, or 2 Channels Differiential
Up to 200kHz Conversion Rate - Limited by arduino hardware
Serial (SPI) Interface
DIP-16 or SSOP-16 Packages Available
Documentation can be found here: http://www.ti.com/lit/ds/symlink/ads7841.pdf
////////////////////
Written by Hybridsix
with great help and input from Darkmoonsinger, and Snail
Based on, but heavily mofified from, some original code written by madvoid
////////////////////
July 1, 2013
*/

#include <SPI.h>

const byte channel0 = B10010100;  //Sets up each channel and Channel function
const byte channel1 = B11010100;  // Bit 7 - Start Bit  | Bit 6-4 Channel Select
const byte channel2 = B10100100;  // Bit 3 - MODE (12bit/8bit) Select 
const byte channel3 = B11100100;  // Bit 2 - Single/Differiential In  | Bit 1-0 Power Mode

void setup() 
{
  Serial.begin(9600);                     // Start serial, rate 9600 baud
  SPI.begin();                            // Start SPI
  digitalWrite (SS, HIGH);                // Sets Chip Select HIGH to clear all selects
  SPI.setClockDivider (SPI_CLOCK_DIV16);  // Set the SPI Clock divisor, setting SPI Speed
}

void loop ()
{
 word data0;                              // Initialize the variables for the return
 word data1;                              // data. 
 word data2;
 word data3;
 
 data0 = ads7841(channel0);               // Sends the control bits to the reader function
   Serial.print("Channel 0 ");            // for the channel, and prints the values  
   Serial.print(data0);                   // returned on the serial port. 
 
 data1 = ads7841(channel1); 
   Serial.print("  | Channel 1 ");        // For channel 1.....
   Serial.print(data1);
 
 data2 = ads7841(channel2);
   Serial.print("  | Channel 2 ");        // For channel 2.....
   Serial.print(data2);
 
 data3 = ads7841(channel3);
   Serial.print("  | Channel 3 ");        // For channel 3.....
   Serial.print(data3);

 Serial.println();                        // sends the LF to make it pretty. 
 delay (500);                              // Delays inbetween read iterations
}

word ads7841(const byte control)          // Read from the ADC function: take in the 
{                                         // channel select byte as "control", return data
  byte MSB;                               // Since the ADC returns more than a single
  byte LSB;                               // byte, we have to use two variables
  word spidata;                           // final combined/processed value
  
  digitalWrite(SS,LOW);                   // Select the ADC, tell it we're ready to talk
    SPI.transfer(control);                // Send the control byte to the ADC
    MSB = SPI.transfer(0);                // Reads the first part of the data
    LSB = SPI.transfer(0);                // Reads the last part of the data
    spidata = MSB << 5 | LSB >> 3;        // Shifts the MSB/LSB Data into a word
    digitalWrite(SS, HIGH);               // Deselect the ADC.
 return (spidata);                        // Returns the restulant value
}
