#ifndef NMEA_INC_NMEA_PARSER_H_
#define NMEA_INC_NMEA_PARSER_H_

#include "nmea_conf.h"
#include "nmea.h"

void NMEA_ParseData(NMEA_HandlerTypeDef *hnmea);
static uint8_t strToInt(uint8_t *s, uint8_t len);
static float strToFloat(uint8_t *s, uint8_t len);
static NMEA_Time_t strToTime(uint8_t *s, uint8_t len);
static NMEA_Date_t strToDate(uint8_t *s, uint8_t len);
static NMEA_Latitude_t strToPosLat(uint8_t *s, uint8_t len);
static NMEA_Longitude_t strToPosLong(uint8_t *s, uint8_t len);

#endif