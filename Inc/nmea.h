#ifndef NMEA_INC_NMEA_H_
#define NMEA_INC_NMEA_H_

#include "nmea_conf.h"
#include "stm32f4xx_hal.h"

#define NMEA_MESSAGE_TYPE_GGA 0x01
#define NMEA_MESSAGE_TYPE_GLL 0x02
#define NMEA_MESSAGE_TYPE_GSA 0x03
#define NMEA_MESSAGE_TYPE_GSV 0x04
#define NMEA_MESSAGE_TYPE_MSS 0x05
#define NMEA_MESSAGE_TYPE_RMC 0x06
#define NMEA_MESSAGE_TYPE_VTG 0x07
#define NMEA_MESSAGE_TYPE_ZDA 0x08

#define NMEA_R_STATE_STOP 0
#define NMEA_R_STATE_GET_HEADER 1
#define NMEA_R_STATE_WRITING 2
#define NMEA_R_STATE_GET_CRC 4
#define NMEA_R_STATE_READY_TO_STOP 5

#define NMEA_BUFFER_MAX_LEN 16

typedef struct NMEA_Time_struct {
  uint8_t hours;
  uint8_t minutes;
  float secconds;
} NMEA_Time_t;

typedef struct NMEA_Date_struct {
  uint8_t year;
  uint8_t month;
  uint8_t day;
} NMEA_Date_t;

typedef struct NMEA_position_struct {
  uint8_t degrees;
  float minutes;
  uint8_t direction;
} NMEA_Position_t, NMEA_Latitude_t, NMEA_Longitude_t;

struct NMEA_GGA_Struct {
  NMEA_Time_t time;
  NMEA_Latitude_t latitude;
  NMEA_Longitude_t longitude;
  uint8_t fix_indicator;
  uint8_t satelites_used;
  float HDOP;
  float MSL;
  uint8_t MSL_Unit;
  float geoidSparation;
  uint8_t geoidSparationUnit;
  float ageOfDiff;
  uint8_t diffRefStationID[4];
};

struct NMEA_GLL_Struct {
  NMEA_Latitude_t latitude;
  NMEA_Longitude_t longitude;
  NMEA_Time_t time;
  char status;
  char mode;
};

struct NMEA_GSA_Struct {
  uint8_t mode1;
  uint8_t mode2;
  uint8_t satelite_used[12];
  float PDOP;
  float HDOP;
  float VDOP;
};

struct NMEA_RMC_Struct {
  NMEA_Time_t time;
  uint8_t status;
  NMEA_Latitude_t latitude;
  NMEA_Longitude_t longitude;
  float speed;
  float course;
  NMEA_Date_t date;
  char magneticVariation;
  char eastWestIndicator;
  char mode;
};

typedef struct NMEA_HandlerStruct {
  uint8_t state;
  uint8_t buffer[NMEA_BUFFER_MAX_LEN];
  uint8_t bufferLen;
  uint8_t messageType;
  uint8_t attrIdx;
  uint8_t crc;

  // event
  void (*onReady)(void*);

  struct {
    struct NMEA_GGA_Struct gga;
    struct NMEA_GLL_Struct gll;
    struct NMEA_GSA_Struct gsa;
    struct NMEA_RMC_Struct rmc;
  } tmpData;

  struct {
    struct NMEA_GGA_Struct gga;
    struct NMEA_GLL_Struct gll;
    struct NMEA_GSA_Struct gsa;
    struct NMEA_RMC_Struct rmc;
  } data;
} NMEA_HandlerTypeDef;

uint8_t NMEA_Read(NMEA_HandlerTypeDef *hnmea, uint8_t *data, uint16_t len);

#endif /* NMEA_INC_NMEA_H_ */
