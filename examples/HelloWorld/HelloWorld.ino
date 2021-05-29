/** Bare minimum example sketch for MP3 player.
 *
 *  Simply plays all tracks in a loop.
 *  It uses SoftwareSerial to connect to the module.
 *
 * @author James Sleeman,  http://sparks.gogo.co.nz/
 * @license MIT License
 * @file
 */
 
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <JQ6500_Serial.h>

// Create the mp3 module object, 
//   Arduino Pin 8 is connected to TX of the JQ6500
//   Arduino Pin 9 is connected to one end of a  1k resistor, 
//     the other end of the 1k resistor is connected to RX of the JQ6500
//   If your Arduino is 3v3 powered, you can omit the 1k series resistor

SoftwareSerial mySerial(8, 9);
JQ6500_Serial mp3(&mySerial);

void setup() {
  
  mySerial.begin(9600);
  mp3.reset();
  mp3.setVolume(20);
  mp3.setLoopMode(MP3_LOOP_ALL);
  mp3.play();  
}

void loop() {
  // Do nothing, it's already playing and looping :-)
}
