/* rawSend.ino Example sketch for IRLib2
 *  Illustrates how to send a code Using raw timings which were captured
 *  from the "rawRecv.ino" sample sketch.  Load that sketch and
 *  capture the values. They will print in the serial monitor. Then you
 *  cut and paste that output into the appropriate section below.
 */
#include <IRLibSendBase.h>    //We need the base code/
#include <IRLib_HashRaw.h>    //Only use raw sender
#include <EEPROM.h>

IRsendRaw mySender;

void readIntArrayFromEEPROM(int address, int numbers[], int arraySize)
{
  int addressIndex = address;
  for (int i = 0; i < arraySize; i++)
  {
    numbers[i] = (EEPROM.read(addressIndex) << 8) + EEPROM.read(addressIndex + 1);
    addressIndex += 2;
  }
}

void setup() {
  Serial.begin(9600);
}
/* Cut and paste the output from "rawRecv.ino" below here. It will 
 * consist of a #define RAW_DATA_LEN statement and an array definition
 * beginning with "uint16_t rawData[RAW_DATA_LEN]= {…" and concludes
 * with "…,1000};"
 */
#define RAW_DATA_LEN 350
unsigned int rawDataOff[RAW_DATA_LEN]={
  510, 1702, 474, 1730, 494, 1702, 422, 1762, 
  510, 1698, 514, 1694, 494, 1674, 514, 1718, 
  454, 666, 490, 634, 454, 682, 490, 630, 
  486, 658, 486, 630, 490, 638, 470, 666, 
  486, 1706, 478, 1706, 482, 1726, 454, 1754, 
  466, 1722, 490, 1718, 458, 1710, 514, 1698, 
  446, 670, 514, 614, 466, 670, 510, 614, 
  450, 686, 510, 630, 494, 630, 502, 614, 
  514, 1698, 470, 1714, 510, 1702, 442, 1742, 
  510, 1682, 466, 1738, 506, 1686, 438, 1766, 
  506, 618, 470, 666, 506, 614, 506, 634, 
  470, 650, 506, 622, 462, 670, 502, 626, 
  462, 1742, 498, 1690, 466, 670, 502, 1690, 
  466, 1738, 502, 618, 498, 1710, 502, 1686, 
  498, 646, 466, 650, 478, 1730, 470, 650, 
  474, 670, 442, 1766, 450, 666, 446, 698, 
  450, 670, 446, 1762, 450, 666, 446, 1766, 
  446, 1738, 446, 698, 446, 1738, 450, 1762, 
  446, 1738, 446, 674, 466, 1742, 446, 674, 
  442, 698, 446, 1742, 442, 698, 446, 674, 
  442, 698, 446, 1742, 438, 706, 442, 1742, 
  438, 706, 438, 1746, 438, 702, 442, 678, 
  438, 1770, 438, 682, 438, 1770, 438, 682, 
  438, 1770, 414, 706, 438, 1770, 414, 1774, 
  438, 7498, 434, 1000};

//#define RAW_DATA_LEN 350
//unsigned int rawDataOn[RAW_DATA_LEN]={
//  5998, 7474, 466, 1718, 470, 1730, 454, 1746, 
//  466, 1766, 394, 1770, 466, 1726, 434, 1774, 
//  466, 1718, 470, 670, 446, 702, 446, 670, 
//  398, 746, 470, 646, 482, 666, 446, 670, 
//  470, 646, 442, 1750, 494, 1710, 422, 1770, 
//  494, 1698, 414, 1790, 498, 1694, 470, 1734, 
//  498, 1690, 542, 622, 478, 642, 478, 638, 
//  498, 646, 506, 614, 434, 710, 506, 610, 
//  486, 658, 486, 1678, 538, 1670, 506, 1678, 
//  478, 1754, 478, 1706, 482, 1726, 454, 1734, 
//  482, 1706, 474, 666, 482, 642, 442, 694, 
//  482, 638, 446, 694, 454, 666, 450, 694, 
//  450, 666, 450, 1762, 454, 662, 478, 642, 
//  470, 1738, 450, 1762, 446, 670, 450, 1738, 
//  446, 1762, 450, 670, 446, 1762, 450, 1738, 
//  442, 698, 450, 670, 446, 1762, 446, 674, 
//  446, 694, 450, 670, 446, 1762, 446, 674, 
//  446, 1762, 446, 1738, 446, 698, 442, 1742, 
//  446, 1742, 466, 1742, 442, 678, 462, 1746, 
//  442, 678, 438, 702, 442, 1746, 438, 706, 
//  438, 682, 434, 730, 414, 1746, 438, 730, 
//  414, 1746, 438, 730, 410, 1750, 434, 734, 
//  410, 706, 414, 1794, 414, 706, 414, 1794, 
//  414, 706, 414, 1770, 438, 706, 414, 1770, 
//  414, 1794, 414, 7550, 414, 1000};

/*
 * Cut-and-paste into the area above.
 */
   
void loop() {
//  if (botaoDesliga.pressed()) {
//    mySender.send(rawDataOff,RAW_DATA_LEN,36);//Pass the buffer,length, optionally frequency
//    Serial.println(F("AC Switched Off"));
//  }

//  if (botaoLiga.pressed()) {
//    mySender.send(rawDataOn,RAW_DATA_LEN,36);//Pass the buffer,length, optionally frequency
//    Serial.println(F("AC Switched On"));
//  }
  delay(1000);
  uint16_t rawDataOn[198];
  readIntArrayFromEEPROM(0, rawDataOn, 198);
  mySender.send(rawDataOn,198,36);
}
