/* Client side utility declarations for esp32-weather-epd.
 * Copyright (C) 2022-2023  Luke Marzen
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

#ifndef __CLIENT_UTILS_H__
#define __CLIENT_UTILS_H__

#include <WiFi.h>
#include <ArduinoJson.h>
#include "api_response.h"

wl_status_t startWiFi(int &wifiRSSI);
void killWiFi();
bool waitForSNTPSync(tm *timeInfo);
bool printLocalTime(tm *timeInfo);

bool makeAPICalls(owm_resp_onecall_t &resp_main, owm_resp_air_pollution_t &resp_pollution);

template <typename T>
int getData(WiFiClient &client, String host, String url, T &r, DeserializationError(*deserializeCall)(Stream&, T&));

#endif