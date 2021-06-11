/** Full demo of MP3 Controls, after uploading, open your Serial Monitor and enter commands.
 * 
 * Allows you to test all the various controls of the JQ6500 module.
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
JQ6500_Serial mp3(mySerial);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
  mp3.reset();
  statusAndHelpOutput();
}

void loop() {
  
    byte b;
    if(Serial.available())
    {
      b = Serial.read();

      switch(b)
      {
        case 'p': Serial.println("Play");   mp3.play();     return;
        case 'r': Serial.println("Restart"); mp3.restart(); return;
        case ' ': Serial.println("Pause"); mp3.pause();     return;
        case '>': Serial.println("Next");  mp3.next();      return;
        case '<': Serial.println("Prev");  mp3.prev();      return;
        
        case ']': Serial.println("Next Folder");  mp3.nextFolder(); return;
        case '[': Serial.println("Prev Folder");  mp3.prevFolder(); return;
    
        case '+': Serial.println("Vol +"); mp3.volumeUp(); return;
        case '-': Serial.println("Vol -"); mp3.volumeDn(); return;
        case 'm': Serial.println("Vol 0"); mp3.setVolume(0); return;
        
        case 'v': 
        {
          char volBuff[10]; 
          memset(volBuff, 0, sizeof(volBuff));          
          Serial.readBytesUntil('\n',volBuff, sizeof(volBuff)-1);
          mp3.setVolume(max(0,min(30, atoi(volBuff))));
          
          Serial.print("Vol ");
          Serial.println(max(0,min(30, atoi(volBuff))));                    
        }
        return;

        case 'e':
        {
          do
          {
            while(!Serial.available()); // Wait
            b = Serial.read();
            if(b != ' ') break; // Allow "e N" or "eN" etc...
          } while(1);
          
          Serial.print("Equalizer ");
          switch(b)
          {
            case 'N': Serial.println("Normal");  mp3.setEqualizer(MP3_EQ_NORMAL);  break;
            case 'P': Serial.println("Pop");     mp3.setEqualizer(MP3_EQ_POP);     break;
            case 'R': Serial.println("Rock");    mp3.setEqualizer(MP3_EQ_ROCK);    break;
            case 'J': Serial.println("Jazz");    mp3.setEqualizer(MP3_EQ_JAZZ);    break;
            case 'C': Serial.println("Classic"); mp3.setEqualizer(MP3_EQ_CLASSIC); break;
            case 'B': Serial.println("Bass");    mp3.setEqualizer(MP3_EQ_BASS);    break;
          }
        }
        return;

        case 'l':
        {
          do
          {
            while(!Serial.available()); // Wait
            b = Serial.read();
            if(b != ' ') break; // Allow "e N" or "eN" etc...
          } while(1);
          
          Serial.print("Loop ");
          switch(b)
          {
            case 'A': Serial.println("All"); mp3.setLoopMode(MP3_LOOP_ALL);      break;  // Plays the tracks one after another and repeats 
            case 'F': Serial.println("Folder"); mp3.setLoopMode(MP3_LOOP_FOLDER);       break; // Loop within folder
            case 'O': Serial.println("One (repeat playing same file)"); mp3.setLoopMode(MP3_LOOP_ONE);       break; // | These seem to do the same, repeat the same track over and over
            case 'R': Serial.println("??? - Don't know what it means exactly, in the datasheet it is \"RAM\""); mp3.setLoopMode(MP3_LOOP_RAM);       break; //- 
            case 'N': 
            case 'S': Serial.println("None (play file and stop)"); mp3.setLoopMode(MP3_LOOP_ONE_STOP); break;  // Default, plays track and stops
          }
        }
        return;

        case 's':
        {
          do
          {
            while(!Serial.available()); // Wait
            b = Serial.read();
            if(b != ' ') break; // Allow "e N" or "eN" etc...
          } while(1);
          
          Serial.print("Source ");
          switch(b)
          {
            case 'S': Serial.println("SD Card (if available)."); mp3.setSource(MP3_SRC_SDCARD);      break;
            case 'B': Serial.println("on board memory.");mp3.setSource(MP3_SRC_BUILTIN);   break;            
          }
        }
        return;

        case 'f':
        {
          char fnumBuff[10]; 
          memset(fnumBuff, 0, sizeof(fnumBuff));          
          Serial.readBytesUntil('\n',fnumBuff, sizeof(fnumBuff)-1);
          unsigned int fnum = strtoul(fnumBuff, NULL, 10);

          Serial.println();
          Serial.print("Play file #");
          Serial.print(fnum);
          Serial.println(F(" (if it exists)."));
          mp3.playFileByIndexNumber(fnum); // 48 == ord('0')   
          return;
          
        }
        return;
        
        case 'F':
        {
          char fnumBuff[10]; 
          memset(fnumBuff, 0, sizeof(fnumBuff));          
          Serial.readBytesUntil('/',fnumBuff, sizeof(fnumBuff)-1);
          unsigned int folnum = strtoul(fnumBuff, NULL, 10);
          
          memset(fnumBuff, 0, sizeof(fnumBuff));          
          Serial.readBytesUntil('\n',fnumBuff, sizeof(fnumBuff)-1);
          unsigned int fnum = strtoul(fnumBuff, NULL, 10);
          
          fnum   = max(1,min(fnum, 999));
          folnum = max(1,min(folnum, 99));
          
          Serial.print("Play "); 
            if(folnum < 10) Serial.print('0');
          Serial.print(folnum);
          Serial.print('/');
            if(fnum < 10) Serial.print("00");
            else if(fnum < 10) Serial.print('0');
          Serial.print(fnum);
          Serial.println(".mp3 (if it exists).");
          mp3.playFileNumberInFolderNumber(folnum, fnum); // 48 == ord('0')   
        }
        return;
                  
        case '?': statusAndHelpOutput(); return;
        
        case 'S': Serial.println("Sleep"); mp3.sleep(); return;
        case 'z': Serial.println("Reset"); mp3.reset(); return;
        
      }
      
    }
    
    static unsigned long m = millis();
    
    if(millis() > 1000 && m < (millis() - 1000))
    {
      if((mp3.getStatus() == MP3_STATUS_PLAYING))
      {
        Serial.print(F("Playing, Current Position: "));
        Serial.print(mp3.currentFilePositionInSeconds());
        Serial.print(F("s / "));
        Serial.print(mp3.currentFileLengthInSeconds());       
        Serial.println('s');
      }
      m = millis();
    }  
}

void statusAndHelpOutput()
{
   Serial.println();
   Serial.println(F("JQ6500 MP3 Player Demo"));
   Serial.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
   
   Serial.print(F("Status           : "));
   switch(mp3.getStatus())
   {
     case MP3_STATUS_STOPPED: Serial.println(F("Stopped")); break;
     case MP3_STATUS_PLAYING: Serial.println(F("Playing")); break;
     case MP3_STATUS_PAUSED:  Serial.println(F("Paused"));  break;
   }
   
   Serial.print(F("Volume (0-30)    : "));
   Serial.println(mp3.getVolume());
  
   Serial.print(F("Equalizer        : "));
   switch(mp3.getEqualizer())
   {
     case MP3_EQ_NORMAL:     Serial.println(F("Normal"));  break;
     case MP3_EQ_POP:        Serial.println(F("Pop"));     break;
     case MP3_EQ_ROCK:       Serial.println(F("Rock"));    break;
     case MP3_EQ_JAZZ:       Serial.println(F("Jazz"));    break;
     case MP3_EQ_CLASSIC:    Serial.println(F("Classic")); break;
     case MP3_EQ_BASS:       Serial.println(F("Bass"));    break;     
   }
   
   Serial.print(F("Loop Mode        : "));
   switch(mp3.getLoopMode())
   {
     case MP3_LOOP_ALL:      Serial.println(F("Play all tracks, then repeat."));  break;
     case MP3_LOOP_FOLDER:   Serial.println(F("Play all tracks in folder, then repeat."));     break;
     case MP3_LOOP_ONE:      Serial.println(F("Play one track then repeat (loop track)."));    break;
     case MP3_LOOP_RAM:      Serial.println(F("Unknown function exactly, seems to play one track then repeat?"));    break;
     case MP3_LOOP_ONE_STOP: Serial.println(F("Play one track then stop."));    break;          
   }
   Serial.println();
    
   Serial.print(F("# of On Board Memory Files    : "));
   Serial.println(mp3.countFiles(MP3_SRC_BUILTIN));
       
   Serial.print(F("\"Current\" On Board Memory File Index: "));
   Serial.println(mp3.currentFileIndexNumber(MP3_SRC_BUILTIN));
   Serial.println();
   
   Serial.print(F("# of SD Card Files    : "));
   Serial.println(mp3.countFiles(MP3_SRC_SDCARD));
   
   Serial.print(F("# of SD Card Folders  : "));
   Serial.println(mp3.countFolders(MP3_SRC_SDCARD));
   
   Serial.print(F("\"Current\" SD Card File Index: "));
   Serial.println(mp3.currentFileIndexNumber(MP3_SRC_SDCARD));
   
   Serial.print(F("\"Current\" SD Card File Name : "));   
   char buff[120];
   mp3.currentFileName(buff, sizeof(buff));
   Serial.println(buff);
   
   Serial.println();
   Serial.println(F("Controls (type in serial monitor and hit send): "));
   Serial.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
   Serial.println(F("? Display this menu.\n"));
   
   Serial.println(F("p Play\t\t> Next\t\t< Prev\n[space] Pause\tr Restart from start of file\n] Next folder\t[ Prev folder\n"));
   
   Serial.println(F("f[1-65534]      Play file by (FAT table) index number\nF[01-99]/[001-999].mp3 Play [001-999].mp3 in folder [01-99]\n"));
   
   Serial.println(F("+ Vol up\t- Vol down\tm Mute\nv[0-30] Set volume\n\ne[N/P/R/J/C/B] Equalizer (N)ormal, (P)op, (R)ock, (J)azz, (C)lassic, (B)ass\nl[A/F/O/R/N]   Loop (A)ll, (F)older, (O)ne, (R)???, (N)o Loop\ns[S/B]         Switch to (S)D Card/(B)uilt In Memory\n\n"));
}
