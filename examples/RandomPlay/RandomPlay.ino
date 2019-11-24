/** Example sketch which plays files on the media in random order.
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
JQ6500_Serial mp3(8,9);

unsigned int numFiles; // Total number of files on media (autodetected in setup())
byte mediaType;        // Media type (autodetected in setup())

void setup() {
  // Start debugging terminal
  Serial.begin(9600);  
  
  // Begin the connection, reset the device, and set volume to a reasonable level
  mp3.begin(9600);
  while(numFiles == 0)
  {
    mp3.reset();  
    
    mp3.setVolume(20);
    mp3.setLoopMode(MP3_LOOP_NONE);
   
    // Try to get the number of files on the SD Card
    numFiles = mp3.countFiles(MP3_SRC_SDCARD);
  
    if(numFiles)
    {
      // If there are SD Card files, make sure we have selected that source    
      mp3.setSource(MP3_SRC_SDCARD);    
      mediaType = MP3_SRC_SDCARD;
    }
    else
    {
      // If none are found, the SD Card is not present or empty, so use
      // the on board memory; Make sure we select the built in source
      mp3.setSource(MP3_SRC_BUILTIN);
      numFiles = mp3.countFiles(MP3_SRC_BUILTIN);
      mediaType = MP3_SRC_BUILTIN;
    }
    
    if(!numFiles)
    {
      Serial.println(F("Error! No files were found on the media, both SD Card and Built In memory were checked."));
      Serial.println(F("We will try again in 3 seconds."));
      Serial.println(F("If there are files there and we still can't find them, try turning everything off and on again, perhaps the module is confused."));
      Serial.println(F("I think this might happen sometimes if you insert/remove an SD Card while powered up, but not totally sure!"));
      Serial.println(F("In a real application, you might consider powering the JQ6500 module through a suitable MOSFET or BJT controlled from a pin so you can power-cycle the JQ6500 if it starts to act weird like this!"));
      delay(3000);
    }
  }
}

void loop() {
  
  //  ** NOTE: Checking for STOPPED doesn't work with the builtin memory 
  //      because in that case the devie appears to only return PAUSED, not STOPPED  
  byte stat = mp3.getStatus();
  if(stat != MP3_STATUS_PLAYING) 
  {
    // pick a random file, numbered 1 to numFiles (NB: random() returns up to but not including the highest number, hence why we add 1)
    // if the file is the same as the one which was just played, pick a different one
    unsigned int pick;
    do
    { 
      pick = random(1,numFiles+1);
    } while(pick == mp3.currentFileIndexNumber(mediaType));
    

    Serial.print("Randomly selected #");
    Serial.print(pick);
    Serial.print(" of ");
    Serial.println(numFiles);

    
    // and play it
    mp3.playFileByIndexNumber(pick);
   
    // if we are in SD Card mode, we can print the file name
    // the built in memory does not have file names
    if(mediaType == MP3_SRC_SDCARD)
    {
      char buffer[20];
      mp3.currentFileName(buffer, sizeof(buffer));
      Serial.print("> ");
      Serial.println(buffer);
    }
  }
}
