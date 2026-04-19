/* Weather service specific declarations for esp32-weather-epd.
 * Copyright (C) 2022-2025  Matteo Battistutta
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

#ifndef __WEATHER_SERVICE_H__
#define __WEATHER_SERVICE_H__

#include <ArduinoJson.h>
#include <WiFiClient.h>
#include "api_response.h"

#if HTTP_MODE == HTTPS_WITH_CERT_VERIF
extern const char* TLS_CERT;
#endif

extern const String SERVICE_NAME;

extern const char* DOMAIN_MAIN;
extern const char* DOMAIN_POLLUTION;

String buildMainURL();
String buildPollutionURL(char* startStr, char* endStr);
String getAPIKeyParam(String key);
DeserializationError deserializeMainCall(Stream &json,
                                        owm_resp_onecall_t &r);
DeserializationError deserializeAirQuality(Stream &json,
                                           owm_resp_air_pollution_t &r);

#endif
