/** 
 * Arduino Library for JQ6500 MP3 Module
 * 
 * Copyright (C) 2014 James Sleeman, <http://sparks.gogo.co.nz/jq6500/index.html>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 * THE SOFTWARE.
 * 
 * @author James Sleeman, http://sparks.gogo.co.nz/
 * @license MIT License
 * @file
 */

#include <Arduino.h>
#include "JQ6500_Serial_ESP32.h"



void  JQ6500_Serial_ESP32::play()
{
  this->sendCommand(0x0D);
}

void  JQ6500_Serial_ESP32::restart()
{
  byte oldVolume = this->getVolume();  
  this->setVolume(0);
  this->next();
  this->pause();
  this->setVolume(oldVolume);
  this->prev();
}

void  JQ6500_Serial_ESP32::pause()
{
  this->sendCommand(0x0E);
}

void  JQ6500_Serial_ESP32::next()
{
  this->sendCommand(0x01);
}

void  JQ6500_Serial_ESP32::prev()
{
  this->sendCommand(0x02);
}

void  JQ6500_Serial_ESP32::playFileByIndexNumber(unsigned int fileNumber)
{  
  this->sendCommand(0x03, (fileNumber>>8) & 0xFF, fileNumber & (byte)0xFF);
}

void  JQ6500_Serial_ESP32::nextFolder()
{
  this->sendCommand(0x0F, 0x01);  
}

void  JQ6500_Serial_ESP32::prevFolder()
{
  this->sendCommand(0x0F, 0x00);
}

void  JQ6500_Serial_ESP32::playFileNumberInFolderNumber(unsigned int folderNumber, unsigned int fileNumber)
{
  this->sendCommand(0x12, folderNumber & 0xFF, fileNumber & 0xFF);
}

void  JQ6500_Serial_ESP32::volumeUp()
{
  this->sendCommand(0x04);
}

void  JQ6500_Serial_ESP32::volumeDn()
{
  this->sendCommand(0x05);
}

void  JQ6500_Serial_ESP32::setVolume(byte volumeFrom0To30)
{
  this->sendCommand(0x06, volumeFrom0To30);
}

void  JQ6500_Serial_ESP32::setEqualizer(byte equalizerMode)
{
  this->sendCommand(0x07, equalizerMode);
}

void  JQ6500_Serial_ESP32::setLoopMode(byte loopMode)
{
  this->sendCommand(0x11, loopMode);
}

void  JQ6500_Serial_ESP32::setSource(byte source)
{
  this->sendCommand(0x09, source);
}

void  JQ6500_Serial_ESP32::sleep()
{
  this->sendCommand(0x0A);
}

void  JQ6500_Serial_ESP32::reset()
{
  this->sendCommand(0x0C);
  delay(500); // We need some time for the reset to happen
}


    byte  JQ6500_Serial_ESP32::getStatus()    
    {
      byte statTotal = 0;
      byte stat       = 0;
      do
      {
        statTotal = 0;
        for(byte x = 0; x < MP3_STATUS_CHECKS_IN_AGREEMENT; x++)
        {
          stat = this->sendCommandWithUnsignedIntResponse(0x42);      
          if(stat == 0) return 0; // STOP is fairly reliable
          statTotal += stat;
        }

      } while (statTotal != 1 * MP3_STATUS_CHECKS_IN_AGREEMENT && statTotal != 2 * MP3_STATUS_CHECKS_IN_AGREEMENT);
      
      return statTotal / MP3_STATUS_CHECKS_IN_AGREEMENT;      
    }
    
    byte  JQ6500_Serial_ESP32::getVolume()    { return this->sendCommandWithUnsignedIntResponse(0x43); }
    byte  JQ6500_Serial_ESP32::getEqualizer() { return this->sendCommandWithUnsignedIntResponse(0x44); }
    byte  JQ6500_Serial_ESP32::getLoopMode()  { return this->sendCommandWithUnsignedIntResponse(0x45); }
    unsigned int  JQ6500_Serial_ESP32::getVersion()   { return this->sendCommandWithUnsignedIntResponse(0x46); }
    
    unsigned int  JQ6500_Serial_ESP32::countFiles(byte source)   
    {
      if(source == MP3_SRC_SDCARD)
      {
        return this->sendCommandWithUnsignedIntResponse(0x47); 
      }
      else if (source == MP3_SRC_BUILTIN)
      {
        return this->sendCommandWithUnsignedIntResponse(0x49);
      }     
      
      return 0;
    }
    
    unsigned int  JQ6500_Serial_ESP32::countFolders(byte source) 
    {
      if(source == MP3_SRC_SDCARD)
      {
        return this->sendCommandWithUnsignedIntResponse(0x53); 
      }
      
      return 0;
    }
    
    unsigned int  JQ6500_Serial_ESP32::currentFileIndexNumber(byte source)
    {
      if(source == MP3_SRC_SDCARD)
      {
        return this->sendCommandWithUnsignedIntResponse(0x4B); 
      }
      else if (source == MP3_SRC_BUILTIN)
      {
        return this->sendCommandWithUnsignedIntResponse(0x4D)+1; // CRAZY!
      }     
      
      return 0;
    }
    
    unsigned int  JQ6500_Serial_ESP32::currentFilePositionInSeconds() { return this->sendCommandWithUnsignedIntResponse(0x50); }
    unsigned int  JQ6500_Serial_ESP32::currentFileLengthInSeconds()   { return this->sendCommandWithUnsignedIntResponse(0x51); }
    
    void          JQ6500_Serial_ESP32::currentFileName(char *buffer, unsigned int bufferLength) 
    {
      this->sendCommand(0x52, 0, 0, buffer, bufferLength);
    }
    
    // Used for the status commands, they mostly return an 8 to 16 bit integer 
    // and take no arguments
    unsigned int JQ6500_Serial_ESP32::sendCommandWithUnsignedIntResponse(byte command)
    {      
      char buffer[5];
      this->sendCommand(command, 0, 0, buffer, sizeof(buffer));
      return (unsigned int) strtoul(buffer, NULL, 16);
    }
    
    void  JQ6500_Serial_ESP32::sendCommand(byte command)
    {
      this->sendCommand(command, 0, 0, 0, 0);
    }
    
    void  JQ6500_Serial_ESP32::sendCommand(byte command, byte arg1)
    {
       this->sendCommand(command, arg1, 0, 0, 0);
    }
    
    void  JQ6500_Serial_ESP32::sendCommand(byte command, byte arg1, byte arg2)
    {
       this->sendCommand(command, arg1, arg2, 0, 0);
    }    
    
    void  JQ6500_Serial_ESP32::sendCommand(byte command, byte arg1, byte arg2, char *responseBuffer, unsigned int bufferLength)
    {
      
      
      // Command structure
      // [7E][number bytes following including command and terminator][command byte][?arg1][?arg2][EF]
      
      // Most commands do not have arguments
      byte args = 0;
      
      // These ones do
      switch(command)
      {        
        case 0x03: args = 2; break;
        case 0x06: args = 1; break;
        case 0x07: args = 1; break;        
        case 0x09: args = 1; break;
        case 0x0F: args = 1; break;
        case 0x11: args = 1; break;
        case 0x12: args = 2; break;
      }
      
#if MP3_DEBUG
      char buf[4];       
      Serial.println();
      Serial.print("7E ");      
      itoa(2+args, buf, 16); Serial.print(buf); Serial.print(" "); memset(buf, 0, sizeof(buf));
      itoa(command, buf, 16); Serial.print(buf); Serial.print(" "); memset(buf, 0, sizeof(buf));
      if(args>=1) itoa(arg1, buf, 16); Serial.print(buf); Serial.print(" "); memset(buf, 0, sizeof(buf));
      if(args>=2) itoa(arg2, buf, 16); Serial.print(buf); Serial.print(" "); memset(buf, 0, sizeof(buf));
      Serial.print("EF");      
#endif
      
      // The device appears to send some sort of status information (namely "STOP" when it stops playing)
      // just discard this right before we send the command
      while(this->waitUntilAvailable(10)) this->read();
      
      this->write((byte)0x7E);
      this->write(2+args);
      this->write(command);
      if(args>=1) this->write(arg1);
      if(args==2) this->write(arg2);
      this->write((byte)0xEF);
      
      if(!responseBuffer && !bufferLength) return;
      
      
      unsigned int i = 0;
      char         j = 0;
      if(responseBuffer && bufferLength) 
      {
        memset(responseBuffer, 0, bufferLength);
      }
      
      // Allow some time for the device to process what we did and 
      // respond, up to 1 second, but typically only a few ms.
      this->waitUntilAvailable(1000);

      
#if MP3_DEBUG
      Serial.print(" ==> [");
#endif
      
      while(this->waitUntilAvailable(150))
      {
        j = (char)this->read();
        
#if MP3_DEBUG
        Serial.print(j);
#endif
        if(responseBuffer && (i<(bufferLength-1)))
        {
           responseBuffer[i++] = j;
        }
      }
      
#if MP3_DEBUG      
      Serial.print("]");
      Serial.println();
#endif
      
    }
    
    
// as readBytes with terminator character
// terminates if length characters have been read, timeout, or if the terminator character  detected
// returns the number of characters placed in the buffer (0 means no valid data found)

size_t JQ6500_Serial_ESP32::readBytesUntilAndIncluding(char terminator, char *buffer, size_t length, byte maxOneLineOnly)
{
    if (length < 1) return 0;
  size_t index = 0;
  while (index < length) {
    int c = timedRead();
    if (c < 0) break;    
    *buffer++ = (char)c;
    index++;
    if(c == terminator) break;
    if(maxOneLineOnly && ( c == '\n') ) break;
  }
  return index; // return number of characters, not including null terminator
}


// Waits until data becomes available, or a timeout occurs
int JQ6500_Serial_ESP32::waitUntilAvailable(unsigned long maxWaitTime)
{
  unsigned long startTime;
  int c = 0;
  startTime = millis();
  do {
    c = this->available();
    if (c) break;
  } while(millis() - startTime < maxWaitTime);
  
  return c;
}
