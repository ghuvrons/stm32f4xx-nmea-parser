#include "nmea_conf.h"
#include "nmea.h"
#include "nmea_parser.h"
#include <stdio.h>
#include <string.h>

#define WRITE_BUFF(hnmea, data) {\
  (hnmea)->buffer[(hnmea)->bufferLen] = (data); \
  (hnmea)->bufferLen++; \
}
#define ADD_CRC(hnmea, data) {\
  (hnmea)->crc ^= (data); \
}

uint8_t NMEA_Read(NMEA_HandlerTypeDef *hnmea, uint8_t *data, uint16_t len)
{
  while(len){
    switch(*data)
    {
    case '$':
      if(hnmea->state == NMEA_R_STATE_STOP){
        hnmea->state = NMEA_R_STATE_WRITING;
        
        // reset data
        hnmea->bufferLen = 0;
        hnmea->messageType = 0;
        hnmea->attrIdx = 0;
        hnmea->crc = 0;
      }
      break;

    case '*':
      if(hnmea->state == NMEA_R_STATE_WRITING){
        hnmea->state = NMEA_R_STATE_GET_CRC;
      }
    case ',':
      if(hnmea->state == NMEA_R_STATE_WRITING || hnmea->state == NMEA_R_STATE_GET_CRC){
        if(hnmea->state == NMEA_R_STATE_WRITING){
          ADD_CRC(hnmea, *data);
        }
        
        NMEA_ParseData(hnmea);
        hnmea->bufferLen = 0;
        hnmea->attrIdx++;
      }
      break;
      
    case '\r':
      if(hnmea->state == NMEA_R_STATE_GET_CRC){
        hnmea->state = NMEA_R_STATE_READY_TO_STOP;
      }
      break;

    case '\n':
      if(hnmea->state == NMEA_R_STATE_READY_TO_STOP){
        // check crc
        uint8_t crc;
        WRITE_BUFF(hnmea, 0);
        sscanf(hnmea->buffer, "%2hhx", &crc);

        // copy data from temporary data
        if(hnmea->crc == crc){
          switch (hnmea->messageType)
          {
          case NMEA_MESSAGE_TYPE_GGA:
            hnmea->data.gga = hnmea->tmpData.gga;
            break;
          case NMEA_MESSAGE_TYPE_GLL:
            hnmea->data.gll = hnmea->tmpData.gll;
            break;
          case NMEA_MESSAGE_TYPE_GSA:
            hnmea->data.gsa = hnmea->tmpData.gsa;
            break;
          case NMEA_MESSAGE_TYPE_RMC:
            hnmea->data.rmc = hnmea->tmpData.rmc;
            break;
          }
        }

        // reset
        hnmea->bufferLen = 0;
        hnmea->state = NMEA_R_STATE_STOP;
      }
      break;

    default:
      if(hnmea->state == NMEA_R_STATE_WRITING || hnmea->state == NMEA_R_STATE_GET_CRC){
        WRITE_BUFF(hnmea, *data);
        if(hnmea->state == NMEA_R_STATE_WRITING)
          ADD_CRC(hnmea, *data);
      }
      break;
    }
    data++;
    len--;
  }
  return (hnmea->state == NMEA_R_STATE_STOP);
}