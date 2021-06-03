/** Demonstrate how to play a file by it's (FAT table) index number.
 *
 * @author James Sleeman,  http://sparks.gogo.co.nz/
 * @license MIT License
 * @file
 */
 
#include <Arduino.h>
#include <JQ6500_Serial.h>

// Create the mp3 module object. In this example we're using the Hardware Serial port, so:
// Arduino Pin RX is connected to TX of the JQ6500
// Arduino Pin TX is connected to one end of a  1k resistor, 
// the other end of the 1k resistor is connected to RX of the JQ6500
// If your Arduino is 3v3 powered, you can omit the 1k series resistor
JQ6500_Serial mp3(Serial);

void setup() {
  Serial.begin(9600);
  mp3.reset();
  mp3.setVolume(20);
  mp3.setLoopMode(MP3_LOOP_NONE);

}

void loop() {
  
  if(mp3.getStatus() != MP3_STATUS_PLAYING)
  {
    mp3.playFileByIndexNumber(1);  
  } 
}
