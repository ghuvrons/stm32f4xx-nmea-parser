#include "nmea_conf.h"
#include "nmea.h"
#include "nmea_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void NMEA_ParseData(NMEA_HandlerTypeDef *hnmea)
{
  if(hnmea->attrIdx == 0 && hnmea->bufferLen >= 5){
    // check header to get message type
    if(memcmp(hnmea->buffer, "GPGGA", 5) == 0){
      hnmea->messageType = NMEA_MESSAGE_TYPE_GGA;
      memset((void*) &(hnmea->tmpData.gga), 0, sizeof(struct NMEA_GGA_Struct));
    }
    else if (memcmp(hnmea->buffer, "GPGLL", 5) == 0) {
      hnmea->messageType = NMEA_MESSAGE_TYPE_GLL;
      memset((void*) &(hnmea->tmpData.gll), 0, sizeof(struct NMEA_GLL_Struct));
    }
    else if (memcmp(hnmea->buffer, "GPGSA", 5) == 0) {
      hnmea->messageType = NMEA_MESSAGE_TYPE_GSA;
      memset((void*) &(hnmea->tmpData.gsa), 0, sizeof(struct NMEA_GSA_Struct));
    }
    else if (memcmp(hnmea->buffer, "GPGSV", 5) == 0) {
      hnmea->messageType = NMEA_MESSAGE_TYPE_GSV;
    }
    else if (memcmp(hnmea->buffer, "GPMSS", 5) == 0) {
      hnmea->messageType = NMEA_MESSAGE_TYPE_MSS;
    }
    else if (memcmp(hnmea->buffer, "GPRMC", 5) == 0) {
      hnmea->messageType = NMEA_MESSAGE_TYPE_RMC;
      memset((void*) &(hnmea->tmpData.rmc), 0, sizeof(struct NMEA_RMC_Struct));
    }
    else if (memcmp(hnmea->buffer, "GPVTG", 5) == 0) {
      hnmea->messageType = NMEA_MESSAGE_TYPE_VTG;
    }
    else if (memcmp(hnmea->buffer, "GPZDA", 5) == 0) {
      hnmea->messageType = NMEA_MESSAGE_TYPE_ZDA;
    }
    else {
      hnmea->state = NMEA_R_STATE_STOP;
    }
  }
  else {
    switch (hnmea->messageType)
    {
    // Parsing GGA Data
    case NMEA_MESSAGE_TYPE_GGA:
      switch (hnmea->attrIdx)
      {
      case 1:
        hnmea->tmpData.gga.time = strToTime(hnmea->buffer, hnmea->bufferLen);
        break;
      case 2:
        hnmea->tmpData.gga.latitude = strToPosLat(hnmea->buffer, hnmea->bufferLen);
        break;
      case 3:
        memcpy(&(hnmea->tmpData.gga.latitude.direction), hnmea->buffer, hnmea->bufferLen);
        break;
      case 4:
        hnmea->tmpData.gga.longitude = strToPosLong(hnmea->buffer, hnmea->bufferLen);
        break;
      case 5:
        memcpy(&(hnmea->tmpData.gga.longitude.direction), hnmea->buffer, hnmea->bufferLen);
        break;
      case 6:
        hnmea->tmpData.gga.fix_indicator = strToInt(hnmea->buffer, hnmea->bufferLen);
        break;
      case 7:
        hnmea->tmpData.gga.satelites_used = strToInt(hnmea->buffer, hnmea->bufferLen);
        break;
      case 8:
        hnmea->tmpData.gga.HDOP = strToFloat(hnmea->buffer, hnmea->bufferLen);
        break;
      case 9:
        hnmea->tmpData.gga.MSL = strToFloat(hnmea->buffer, hnmea->bufferLen);
        break;
      case 10:
        memcpy(&(hnmea->tmpData.gga.MSL_Unit), hnmea->buffer, hnmea->bufferLen);
        break;
      case 11:
        hnmea->tmpData.gga.geoidSparation = strToFloat(hnmea->buffer, hnmea->bufferLen);
        break;
      case 12:
        memcpy(&(hnmea->tmpData.gga.geoidSparationUnit), hnmea->buffer, hnmea->bufferLen);
        break;
      case 13:
        hnmea->tmpData.gga.ageOfDiff = strToInt(hnmea->buffer, hnmea->bufferLen);
        break;
      case 14:
        memcpy(hnmea->tmpData.gga.diffRefStationID, hnmea->buffer, hnmea->bufferLen);
        break;
      default:
        break;
      }
      break;
    
    // Parsing GLL Data
    case NMEA_MESSAGE_TYPE_GLL:
      switch (hnmea->attrIdx)
      {
      case 1:
        hnmea->tmpData.gll.latitude = strToPosLat(hnmea->buffer, hnmea->bufferLen);
        break;
      case 2:
        memcpy(&(hnmea->tmpData.gll.latitude.direction), hnmea->buffer, hnmea->bufferLen);
        break;
      case 3:
        hnmea->tmpData.gll.longitude = strToPosLong(hnmea->buffer, hnmea->bufferLen);
        break;
      case 4:
        memcpy(&(hnmea->tmpData.gll.longitude.direction), hnmea->buffer, hnmea->bufferLen);
        break;
      case 5:
        hnmea->tmpData.gll.time = strToTime(hnmea->buffer, hnmea->bufferLen);
        break;
      case 6:
        memcpy(&(hnmea->tmpData.gll.status), hnmea->buffer, hnmea->bufferLen);
        break;
      case 7:
        memcpy(&(hnmea->tmpData.gll.mode), hnmea->buffer, hnmea->bufferLen);
        break;
      }
      break;

      
    // Parsing GLL Data
    case NMEA_MESSAGE_TYPE_GSA:
      switch (hnmea->attrIdx)
      {
      case 1:
        memcpy(&(hnmea->tmpData.gsa.mode1), hnmea->buffer, hnmea->bufferLen);
        break;
      case 2:
        hnmea->tmpData.gsa.mode2 = (uint8_t) strToInt(hnmea->buffer, hnmea->bufferLen);
        break;
      case 15:
        hnmea->tmpData.gsa.PDOP = strToFloat(hnmea->buffer, hnmea->bufferLen);
        break;
      case 16:
        hnmea->tmpData.gsa.HDOP = strToFloat(hnmea->buffer, hnmea->bufferLen);
        break;
      case 17:
        hnmea->tmpData.gsa.VDOP = strToFloat(hnmea->buffer, hnmea->bufferLen);
        break;
      default:
        if(hnmea->attrIdx > 2 && hnmea->attrIdx < 15){
          hnmea->tmpData.gsa.satelite_used[hnmea->attrIdx - 3] = (uint8_t) strToInt(hnmea->buffer, hnmea->bufferLen);
        }
        break;
      }
      break;

    case NMEA_MESSAGE_TYPE_RMC:
      switch (hnmea->attrIdx)
      {
      case 1:
        hnmea->tmpData.rmc.time = strToTime(hnmea->buffer, hnmea->bufferLen);
        break;
      case 2:
        memcpy(&(hnmea->tmpData.rmc.status), hnmea->buffer, hnmea->bufferLen);
        break;
      case 3:
        hnmea->tmpData.rmc.latitude = strToPosLat(hnmea->buffer, hnmea->bufferLen);
        break;
      case 4:
        memcpy(&(hnmea->tmpData.rmc.latitude.direction), hnmea->buffer, hnmea->bufferLen);
        break;
      case 5:
        hnmea->tmpData.rmc.longitude = strToPosLong(hnmea->buffer, hnmea->bufferLen);
        break;
      case 6:
        memcpy(&(hnmea->tmpData.rmc.longitude.direction), hnmea->buffer, hnmea->bufferLen);
        break;
      case 7:
        hnmea->tmpData.rmc.speed = strToFloat(hnmea->buffer, hnmea->bufferLen);
        break;
      case 8:
        hnmea->tmpData.rmc.course = strToFloat(hnmea->buffer, hnmea->bufferLen);
        break;
      case 9:
        hnmea->tmpData.rmc.date = strToDate(hnmea->buffer, hnmea->bufferLen);
        break;
      case 10:
        memcpy(&(hnmea->tmpData.rmc.magneticVariation), hnmea->buffer, hnmea->bufferLen);
        break;
      case 11:
        memcpy(&(hnmea->tmpData.rmc.eastWestIndicator), hnmea->buffer, hnmea->bufferLen);
        break;
      case 12:
        memcpy(&(hnmea->tmpData.rmc.mode), hnmea->buffer, hnmea->bufferLen);
        break;
      }
      break;

    default:
      break;
    }
  }
}

static uint8_t strToInt(uint8_t *s, uint8_t len)
{
  uint8_t result = 0;
  char tmp[4] = {0};

  if(len < 1) return 0;
  memcpy(tmp, s, len);
  result = (uint8_t) atoi(tmp);

  return result;
}

static float strToFloat(uint8_t *s, uint8_t len)
{
  float result = 0;
  char tmp[16] = {0};

  if(len < 1) return 0;
  memcpy(tmp, s, len);
  result = (float) atof(tmp);

  return result;
}

/**
 * s format is hhmmss.sss
 */
static NMEA_Time_t strToTime(uint8_t *s, uint8_t len)
{
  NMEA_Time_t result = {0};
  char tmp[7] = {0};

  if(len < 6) return result;

  // get hours
  memcpy(tmp, s, 2);
  result.hours = (uint8_t) atoi(tmp);
  s += 2;

  // get minutes
  memcpy(tmp, s, 2);
  result.minutes = (uint8_t) atoi(tmp);
  s += 2;

  // get seconds
  memcpy(tmp, s, len-4);
  result.secconds = (float) atof(tmp);

  return result;
}

/**
 * s format is ddmmyy
 */
static NMEA_Date_t strToDate(uint8_t *s, uint8_t len)
{
  NMEA_Date_t result = {0};
  char tmp[7] = {0};

  if(len < 6) return result;

  // get hours
  memcpy(tmp, s, 2);
  result.day = (uint8_t) atoi(tmp);
  s += 2;

  // get minutes
  memcpy(tmp, s, 2);
  result.month = (uint8_t) atoi(tmp);
  s += 2;

  // get seconds
  memcpy(tmp, s, 2);
  result.year = (uint8_t) atoi(tmp);

  return result;
}

/**
 * s format is ddmm.mmmm
 */
static NMEA_Latitude_t strToPosLat(uint8_t *s, uint8_t len)
{
  NMEA_Latitude_t result = {0};
  char tmp[8] = {0};

  if(len < 4) return result;

  // get degrees
  memcpy(tmp, s, 2);
  result.degrees = (uint8_t) atoi(tmp);
  s += 2;

  // get minutes
  memcpy(tmp, s, len-2);
  result.minutes = (float) atof(tmp);

  return result;
}

/**
 * s format is dddmm.mmmm
 */
static NMEA_Longitude_t strToPosLong(uint8_t *s, uint8_t len)
{
  NMEA_Longitude_t result;
  char tmp[8] = {0};

  if(len < 5) return result;

  // get degrees
  memcpy(tmp, s, 3);
  result.degrees = (uint8_t) atoi(tmp);
  s += 3;

  // get minutes
  tmp[2] = 0;
  memcpy(tmp, s, len-2);
  result.minutes = (float) atof(tmp);
  
  return result;
}