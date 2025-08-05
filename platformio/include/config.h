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

// DEFAULT CONFIGS

#define HTTP 0
#define HTTPS_NO_CERT_VERIF 1
#define HTTPS_WITH_CERT_VERIF 2
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

// NON-VOLATILE STORAGE (NVS) NAMESPACE
#ifndef NVS_NAMESPACE
  #define NVS_NAMESPACE "weather_epd"
#endif

// Set the below constants in "config.cpp"
extern const char *WIFI_SSID;
extern const unsigned long WIFI_TIMEOUT;
extern const unsigned HTTP_CLIENT_TCP_TIMEOUT;
extern const String OWM_APIKEY;
extern const String OWM_ENDPOINT;
extern const String OWM_ONECALL_VERSION;
extern const String LAT;
extern const String LON;
extern const String CITY_STRING;
extern const char *TIME_FORMAT;
extern const char *HOUR_FORMAT;
extern const char *NTP_SERVER_1;
extern const char *NTP_SERVER_2;
extern const unsigned long NTP_TIMEOUT;
extern const uint32_t WARN_BATTERY_VOLTAGE;
extern const uint32_t LOW_BATTERY_VOLTAGE;
extern const uint32_t VERY_LOW_BATTERY_VOLTAGE;
extern const uint32_t CRIT_LOW_BATTERY_VOLTAGE;
extern const unsigned long LOW_BATTERY_SLEEP_INTERVAL;
extern const unsigned long VERY_LOW_BATTERY_SLEEP_INTERVAL;
extern const uint32_t MAX_BATTERY_VOLTAGE;
extern const uint32_t MIN_BATTERY_VOLTAGE;

#endif
