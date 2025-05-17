/* Configuration option declarations for esp32-weather-epd.
 * Copyright (C) 2022-2025  Luke Marzen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <cstdint>
#include <Arduino.h>

#include "define_values.h"
#include "config_custom.h"

#ifdef USE_IMPERIAL_UNITS
  #ifndef UNITS_TEMP
    #define UNITS_TEMP FAHRENHEIT
  #endif
  #ifndef UNITS_SPEED
    #define UNITS_SPEED MILESPERHOUR
  #endif
  #ifndef UNITS_PRES
    #define UNITS_PRES INCHESOFMERCURY
  #endif
  #ifndef UNITS_DIST
    #define UNITS_DIST MILES
  #endif
  #ifndef UNITS_DAILY_PRECIP
    #define UNITS_DAILY_PRECIP INCHES
  #endif
#endif

// DEFAULT CONFIGS

// E-PAPER PANEL
// This project supports the following E-Paper panels:
//   DISP_BW_V2 - 7.5in e-Paper (v2)      800x480px  Black/White
//   DISP_3C_B  - 7.5in e-Paper (B)       800x480px  Red/Black/White
//   DISP_7C_F  - 7.3in ACeP e-Paper (F)  800x480px  7-Color
//   DISP_BW_V1 - 7.5in e-Paper (v1)      640x384px  Black/White
#ifndef EPD_PANEL
  #define EPD_PANEL DISP_BW_V2
#endif

// E-PAPER DRIVER BOARD
// The DESPI-C02 is the only officially supported driver board.
// Support for the Waveshare rev2.2 and rev2.3 is deprecated.
// The Waveshare rev2.2 is no longer in production.
// Users of the Waveshare rev2.3 have reported experiencing low contrast issues.
#ifndef EPD_DRIVER
  #define EPD_DRIVER DRIVER_DESPI_C02
#endif

// INDOOR ENVIRONMENT SENSOR
#ifndef SENSOR
  #define SENSOR BME280
#endif

// 3 COLOR E-INK ACCENT COLOR
// Defines the 3rd color to be used when a 3+ color display is selected.
#if (EPD_PANEL == DISP_3C_B || EPD_PANEL == DISP_7C_F) && !(defined(ACCENT_COLOR))
  #define ACCENT_COLOR GxEPD_RED
#endif

// UNITS - TEMPERATURE
//   Metric   : Celsius
//   Imperial : Fahrenheit
#ifndef UNITS_TEMP
  #define UNITS_TEMP CELSIUS
#endif

// UNITS - WIND SPEED
//   Metric   : Kilometers per Hour
//   Imperial : Miles per Hour
#ifndef UNITS_SPEED
  #define UNITS_SPEED KILOMETERSPERHOUR
#endif

// UNITS - PRESSURE
//   Metric   : Millibars
//   Imperial : Inches of Mercury
#ifndef UNITS_PRES
  #define UNITS_PRES MILLIBARS
#endif

// UNITS - VISIBILITY DISTANCE
//   Metric   : Kilometers
//   Imperial : Miles
#ifndef UNITS_DIST
  #define UNITS_DIST KILOMETERS
#endif

// UNITS - PRECIPITATION (HOURLY)
// Measure of precipitation.
// This can either be Probability of Precipitation (PoP) or hourly volume.
//   Metric   : Millimeters
//   Imperial : Inches
#ifndef UNITS_HOURLY_PRECIP
  #define UNITS_HOURLY_PRECIP POP
#endif

// UNITS - PRECIPITATION (DAILY)
// Measure of precipitation.
// This can either be Probability of Precipitation (PoP) or daily volume.
//   Metric   : Millimeters
//   Imperial : Inches
#ifndef UNITS_DAILY_PRECIP
  #define UNITS_DAILY_PRECIP MILLIMETERS
#endif

// Hypertext Transfer Protocol (HTTP)
// HTTP
//   HTTP does not provide encryption or any security measures, making it highly
//   vulnerable to eavesdropping and data tampering. Has the advantage of using
//   less power.
// HTTPS_NO_CERT_VERIF
//   HTTPS without X.509 certificate verification provides encryption but lacks
//   authentication and is susceptible to man-in-the-middle attacks.
// HTTPS_WITH_CERT_VERIF
//   HTTPS with X.509 certificate verification offers the highest level of
//   security by providing encryption and verifying the identity of the server.
//
//   HTTPS with X.509 certificate verification comes with the draw back that
//   eventually the certificates on the esp32 will expire, requiring you to
//   update the certificates in cert.h and reflash this software.
//   Running cert.py will generate an updated cert.h file.
//   The current certificate for api.openweathermap.org is valid until
//   2026-04-10 23:59:59+00:00
#ifndef HTTP_MODE
  #define HTTP_MODE HTTPS_WITH_CERT_VERIF
#endif

// WIND DIRECTION LABEL
// Choose whether the wind direction label should be a number or
// expressed in Compass Point Notation (CPN).
// Set the macro to 0 to disable.
//
//   PRECISION                  #     ERROR   EXAMPLE
//   Cardinal                   4  ±45.000°   E
//   Intercardinal (Ordinal)    8  ±22.500°   NE
//   Secondary Intercardinal   16  ±11.250°   NNE
//   Tertiary Intercardinal    32   ±5.625°   NbE
#ifndef WIND_DIRECTION_LABEL
  #define WIND_DIRECTION_LABEL 0
#endif

// WIND DIRECTION ICON PRECISION
// The arrow shown to the left of the wind speed can indicate wind
// direction with a minimum error of ±0.5°. This uses more flash storage because
// 360 24x24 wind direction icons must be stored, totaling ~25kB. For either
// preference or in case flash space becomes a concern there are a handful of
// selectable options listed below. 360 points seems excessive, but the option
// is there.
// Set the macro to 0 to disable.
//
//   PRECISION                  #     ERROR  STORAGE
//   Cardinal                   4  ±45.000°     288B  E
//   Intercardinal (Ordinal)    8  ±22.500°     576B  NE
//   Secondary Intercardinal   16  ±11.250°   1,152B  NNE
//   Tertiary Intercardinal    32   ±5.625°   2,304B  NbE
//   (360)                    360   ±0.500°  25,920B  1°
#ifndef ARROW_PRECISION
  #define ARROW_PRECISION SECONDARY_INTERCARDINAL
#endif

// FONTS
// A handful of popular Open Source typefaces have been included with this
// project for your convenience. Change the font by selecting its corresponding
// header file.
//
//   FONT           HEADER FILE              FAMILY          LICENSE
//   FreeMono       FreeMono.h               GNU FreeFont    GNU GPL v3.0
//   FreeSans       FreeSans.h               GNU FreeFont    GNU GPL v3.0
//   FreeSerif      FreeSerif.h              GNU FreeFont    GNU GPL v3.0
//   Lato           Lato_Regular.h           Lato            SIL OFL v1.1
//   Montserrat     Montserrat_Regular.h     Montserrat      SIL OFL v1.1
//   Open Sans      OpenSans_Regular.h       Open Sans       SIL OFL v1.1
//   Poppins        Poppins_Regular.h        Poppins         SIL OFL v1.1
//   Quicksand      Quicksand_Regular.h      Quicksand       SIL OFL v1.1
//   Raleway        Raleway_Regular.h        Raleway         SIL OFL v1.1
//   Roboto         Roboto_Regular.h         Roboto          Apache v2.0
//   Roboto Mono    RobotoMono_Regular.h     Roboto Mono     Apache v2.0
//   Roboto Slab    RobotoSlab_Regular.h     Roboto Slab     Apache v2.0
//   Ubuntu         Ubuntu_R.h               Ubuntu font     UFL v1.0
//   Ubuntu Mono    UbuntuMono_R.h           Ubuntu font     UFL v1.0
//
// Adding new fonts is relatively straightforward, see fonts/README.
//
// Note:
//   The layout of the display was designed around spacing and size of the GNU
//   FreeSans font, but this project supports the ability to modularly swap
//   fonts. Using a font other than FreeSans may result in undesired spacing or
//   other artifacts.
#ifndef FONT_HEADER
  #define FONT_HEADER "fonts/FreeSans.h"
#endif

// DAILY PRECIPITATION
// Daily precipitation indicated under Hi|Lo can optionally be configured using
// the following options.
//   0 : Disable (hide always)
//   1 : Enable (show always)
//   2 : Smart (show only when precipitation is forecasted)
#ifndef DISPLAY_DAILY_PRECIP
  #define DISPLAY_DAILY_PRECIP 2
#endif

// HOURLY WEATHER ICONS
// Weather icons to be displayed on the temperature and precipitation chart.
// They are drawn at the the x-axis tick marks just above the temperature line
//   0 : Disable
//   1 : Enable
#ifndef DISPLAY_HOURLY_ICONS
  #define DISPLAY_HOURLY_ICONS 1
#endif

// ALERTS
//   The handling of alerts is complex. Each country has a unique national alert
//   system that receives alerts from many different government agencies. This
//   results is huge variance in the formatting of alerts. OpenWeatherMap
//   provides alerts in English only. Any combination of these factors may make
//   it undesirable to display alerts in some regions.
//   Disable alerts by changing the DISPLAY_ALERTS macro to 0.
#ifndef DISPLAY_ALERTS
  #define DISPLAY_ALERTS 1
#endif

// STATUS BAR EXTRAS
//   Extra information that can be displayed on the status bar. Set to 1 to
//   enable.
#ifndef STATUS_BAR_EXTRAS_BAT_VOLTAGE
  #define STATUS_BAR_EXTRAS_BAT_VOLTAGE 0
#endif
#ifndef STATUS_BAR_EXTRAS_WIFI_RSSI
  #define STATUS_BAR_EXTRAS_WIFI_RSSI   0
#endif

// BATTERY MONITORING
//   You may choose to power your weather display with or without a battery.
//   Low power behavior can be controlled in config.cpp.
//   If you wish to disable battery monitoring set this macro to 0.
#ifndef BATTERY_MONITORING
  #define BATTERY_MONITORING 1
#endif

// NON-VOLATILE STORAGE (NVS) NAMESPACE
#ifndef NVS_NAMESPACE
  #define NVS_NAMESPACE "weather_epd"
#endif

// DEBUG
//   If defined, enables increase verbosity over the serial port.
//   level 0: basic status information, assists troubleshooting (default)
//   level 1: increased verbosity for debugging
//   level 2: print api responses to serial monitor
#ifndef DEBUG_LEVEL
  #define DEBUG_LEVEL 0
#endif

// Set the below constants in "config.cpp"
extern const uint8_t PIN_BAT_ADC;
extern const uint8_t PIN_EPD_BUSY;
extern const uint8_t PIN_EPD_CS;
extern const uint8_t PIN_EPD_RST;
extern const uint8_t PIN_EPD_DC;
extern const uint8_t PIN_EPD_SCK;
extern const uint8_t PIN_EPD_MISO;
extern const uint8_t PIN_EPD_MOSI;
extern const uint8_t PIN_EPD_PWR;
extern const uint8_t PIN_BME_SDA;
extern const uint8_t PIN_BME_SCL;
extern const uint8_t PIN_BME_PWR;
extern const uint8_t BME_ADDRESS;
extern const char *WIFI_SSID;
extern const char *WIFI_PASSWORD;
extern const unsigned long WIFI_TIMEOUT;
extern const unsigned HTTP_CLIENT_TCP_TIMEOUT;
extern const String OWM_APIKEY;
extern const String OWM_ENDPOINT;
extern const String OWM_ONECALL_VERSION;
extern const String LAT;
extern const String LON;
extern const String CITY_STRING;
extern const char *TIMEZONE;
extern const char *TIME_FORMAT;
extern const char *HOUR_FORMAT;
extern const char *DATE_FORMAT;
extern const char *REFRESH_TIME_FORMAT;
extern const char *NTP_SERVER_1;
extern const char *NTP_SERVER_2;
extern const unsigned long NTP_TIMEOUT;
extern const int SLEEP_DURATION;
extern const int BED_TIME;
extern const int WAKE_TIME;
extern const int HOURLY_GRAPH_MAX;
extern const uint32_t WARN_BATTERY_VOLTAGE;
extern const uint32_t LOW_BATTERY_VOLTAGE;
extern const uint32_t VERY_LOW_BATTERY_VOLTAGE;
extern const uint32_t CRIT_LOW_BATTERY_VOLTAGE;
extern const unsigned long LOW_BATTERY_SLEEP_INTERVAL;
extern const unsigned long VERY_LOW_BATTERY_SLEEP_INTERVAL;
extern const uint32_t MAX_BATTERY_VOLTAGE;
extern const uint32_t MIN_BATTERY_VOLTAGE;

// CONFIG VALIDATION - DO NOT MODIFY
#if !(  EPD_PANEL == DISP_BW_V2  \
      ^ EPD_PANEL == DISP_3C_B   \
      ^ EPD_PANEL == DISP_7C_F   \
      ^ EPD_PANEL == DISP_BW_V1)
  #error Invalid configuration. Illegal selction of display panel.
#endif
#if !(  EPD_DRIVER == DRIVER_WAVESHARE \
      ^ EPD_DRIVER == DRIVER_DESPI_C02)
  #error Invalid configuration. Illegal selction of driver board.
#endif
#if !(  SENSOR == BME280 \
      ^ SENSOR == BME680)
  #error Invalid configuration. Exactly one sensor must be selected.
#endif
#if !(defined(LOCALE))
  #error Invalid configuration. Locale not selected.
#endif
#if !(  UNITS_TEMP == KELVIN      \
      ^ UNITS_TEMP == CELSIUS     \
      ^ UNITS_TEMP == FAHRENHEIT)
  #error Invalid configuration. Illegal selction of temperature unit.
#endif
#if !(  UNITS_SPEED == METERSPERSECOND   \
      ^ UNITS_SPEED == FEETPERSECOND     \
      ^ UNITS_SPEED == KILOMETERSPERHOUR \
      ^ UNITS_SPEED == MILESPERHOUR      \
      ^ UNITS_SPEED == KNOTS             \
      ^ UNITS_SPEED == BEAUFORT)
  #error Invalid configuration. Illegal selction of wind speed unit.
#endif
#if !(  UNITS_PRES == HECTOPASCALS             \
      ^ UNITS_PRES == PASCALS                  \
      ^ UNITS_PRES == MILLIMETERSOFMERCURY     \
      ^ UNITS_PRES == INCHESOFMERCURY          \
      ^ UNITS_PRES == MILLIBARS                \
      ^ UNITS_PRES == ATMOSPHERES              \
      ^ UNITS_PRES == GRAMSPERSQUARECENTIMETER \
      ^ UNITS_PRES == POUNDSPERSQUAREINCH)
  #error Invalid configuration. Illegal selction of pressure unit.
#endif
#if !(  UNITS_DIST == KILOMETERS \
      ^ UNITS_DIST == MILES)
  #error Invalid configuration. Illegal selction of distance unit.
#endif
#if !(  UNITS_HOURLY_PRECIP == POP         \
      ^ UNITS_HOURLY_PRECIP == MILLIMETERS \
      ^ UNITS_HOURLY_PRECIP == CENTIMETERS \
      ^ UNITS_HOURLY_PRECIP == INCHES)
  #error Invalid configuration. Illegal selction of houly precipitation measurement.
#endif
#if !(  UNITS_DAILY_PRECIP == POP         \
      ^ UNITS_DAILY_PRECIP == MILLIMETERS \
      ^ UNITS_DAILY_PRECIP == CENTIMETERS \
      ^ UNITS_DAILY_PRECIP == INCHES)
  #error Invalid configuration. Illegal selction of daily precipitation measurement.
#endif
#if !(  HTTP_MODE == HTTP                   \
      ^ HTTP_MODE == HTTPS_NO_CERT_VERIF    \
      ^ HTTP_MODE == HTTPS_WITH_CERT_VERIF)
  #error Invalid configuration. Illegal selction of HTTP mode.
#endif
#if !(  WIND_DIRECTION_LABEL == NUMBER                  \
      ^ WIND_DIRECTION_LABEL == CARDINAL                \
      ^ WIND_DIRECTION_LABEL == INTERCARDINAL           \
      ^ WIND_DIRECTION_LABEL == SECONDARY_INTERCARDINAL \
      ^ WIND_DIRECTION_LABEL == TERTIARY_INTERCARDINAL  \
      ^ WIND_DIRECTION_LABEL == 0)
  #error Invalid configuration. Illegal selction of wind indicator(s).
#endif
#if !(  ARROW_PRECISION == CARDINAL                \
      ^ ARROW_PRECISION == INTERCARDINAL           \
      ^ ARROW_PRECISION == SECONDARY_INTERCARDINAL \
      ^ ARROW_PRECISION == TERTIARY_INTERCARDINAL  \
      ^ ARROW_PRECISION == ANY_360  \
      ^ ARROW_PRECISION == 0)
  #error Invalid configuration. Illegal selction of wind direction icon precision level.
#endif
#if !(defined(FONT_HEADER))
  #error Invalid configuration. Font not selected.
#endif
#if !(  DISPLAY_DAILY_PRECIP >= 0 \
     && DISPLAY_DAILY_PRECIP <= 2)
  #error Invalid configuration. Illegal selction of daily precipitation.
#endif
#if !(defined(DISPLAY_HOURLY_ICONS))
  #error Invalid configuration. DISPLAY_HOURLY_ICONS not defined.
#endif
#if !(defined(DISPLAY_ALERTS))
  #error Invalid configuration. DISPLAY_ALERTS not defined.
#endif
#if !(defined(BATTERY_MONITORING))
  #error Invalid configuration. BATTERY_MONITORING not defined.
#endif
#if !(  DEBUG_LEVEL >= 0 \
     && DEBUG_LEVEL <= 2)
  #error Invalid configuration. Illegal selction of DEBUG_LEVEL.
#endif

#endif
