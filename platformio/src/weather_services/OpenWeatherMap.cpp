#if WEATHER_API == OPEN_WEATHER_MAP

#include "weather_service.h"

#include "config.h"
#include "_locale.h"

#if HTTP_MODE == HTTPS_WITH_CERT_VERIF
#include "cert.h"

const char* TLS_CERT = cert_Sectigo_Public_Server_Authentication_Root_R46;
#endif

const String SERVICE_NAME = "One Call " + OWM_ONECALL_VERSION + " API";

const char* DOMAIN_MAIN = "api.openweathermap.org";
const char* DOMAIN_POLLUTION = ENDPOINT_MAIN;

String buildMainURL() {
  String url = "/data/" + OWM_ONECALL_VERSION + "/onecall?lat=" + LAT + "&lon=" + LON + "&lang=" + OWM_LANG + "&units=metric&exclude=minutely";
#if !DISPLAY_ALERTS
  // exclude alerts
  url += ",alerts";
#endif

  return url;
}

String buildPollutionURL(char* startStr, char* endStr) {
  return "/data/2.5/air_pollution/history?lat=" + LAT + "&lon=" + LON + "&start=" + startStr + "&end=" + endStr;
}

String getAPIKeyParam(String key) {
  return "&appid=" + key;
}

DeserializationError deserializeMainCall(Stream &json,
                                        owm_resp_onecall_t &r)
{
  int i;

  JsonDocument filter;
  filter["current"] = true;
  filter["minutely"] = false;
  filter["hourly"] = true;
  filter["daily"] = true;
#if !DISPLAY_ALERTS
  filter["alerts"] = false;
#else
  // description can be very long so they are filtered out to save on memory
  // along with sender_name
  for (int i = 0; i < OWM_NUM_ALERTS; ++i)
  {
    filter["alerts"][i]["sender_name"] = false;
    filter["alerts"][i]["event"] = true;
    filter["alerts"][i]["start"] = true;
    filter["alerts"][i]["end"] = true;
    filter["alerts"][i]["description"] = false;
    filter["alerts"][i]["tags"] = true;
  }
#endif

  JsonDocument doc;

  DeserializationError error = deserializeJson(doc, json,
                                               DeserializationOption::Filter(filter));
#if DEBUG_LEVEL >= 1
  Serial.println("[debug] doc.overflowed() : " + String(doc.overflowed()));
#endif
#if DEBUG_LEVEL >= 2
  serializeJsonPretty(doc, Serial);
#endif
  if (error)
  {
    return error;
  }

  JsonObject current = doc["current"];
  r.current.dt = current["dt"].as<int64_t>();
  r.current.sunrise = current["sunrise"].as<int64_t>();
  r.current.sunset = current["sunset"].as<int64_t>();
  r.current.temp = current["temp"].as<float>();
  r.current.feels_like = current["feels_like"].as<float>();
  r.current.pressure = current["pressure"].as<int>();
  r.current.humidity = current["humidity"].as<int>();
  r.current.dew_point = current["dew_point"].as<float>();
  r.current.clouds = current["clouds"].as<int>();
  r.current.uvi = current["uvi"].as<float>();
  r.current.visibility = current["visibility"].as<int>();
  r.current.wind_speed = current["wind_speed"].as<float>();
  r.current.wind_gust = current["wind_gust"].as<float>();
  r.current.wind_deg = current["wind_deg"].as<int>();
  JsonObject current_weather = current["weather"][0];
  r.current.id = current_weather["id"].as<int>();
  // OpenWeatherMap indicates sun is up with d otherwise n for night
  r.current.is_day = current_weather["icon"].as<String>().endsWith("d");

  i = 0;
  for (JsonObject hourly : doc["hourly"].as<JsonArray>())
  {
    r.hourly[i].dt = hourly["dt"].as<int64_t>();
    r.hourly[i].temp = hourly["temp"].as<float>();
    r.hourly[i].clouds = hourly["clouds"].as<int>();
    r.hourly[i].wind_speed = hourly["wind_speed"].as<float>();
    r.hourly[i].wind_gust = hourly["wind_gust"].as<float>();
    r.hourly[i].pop = hourly["pop"].as<float>() * 100;
    r.hourly[i].precip_1h = hourly["rain"]["1h"].as<float>() + hourly["snow"]["1h"].as<float>();
    JsonObject hourly_weather = hourly["weather"][0];
    r.hourly[i].id = hourly_weather["id"].as<int>();
    // OpenWeatherMap indicates sun is up with d otherwise n for night
    r.hourly[i].is_day = hourly_weather["icon"].as<String>().endsWith("d");

    if (i == OWM_NUM_HOURLY - 1)
    {
      break;
    }
    ++i;
  }

  i = 0;
  for (JsonObject daily : doc["daily"].as<JsonArray>())
  {
    r.daily[i].dt = daily["dt"].as<int64_t>();
    r.daily[i].moonrise = daily["moonrise"].as<int64_t>();
    r.daily[i].moonset = daily["moonset"].as<int64_t>();
    r.daily[i].moon_phase = daily["moon_phase"].as<float>();
    JsonObject daily_temp = daily["temp"];
    r.daily[i].temp.min = daily_temp["min"].as<float>();
    r.daily[i].temp.max = daily_temp["max"].as<float>();
    r.daily[i].clouds = daily["clouds"].as<int>();
    r.daily[i].wind_speed = daily["wind_speed"].as<float>();
    r.daily[i].wind_gust = daily["wind_gust"].as<float>();
    r.daily[i].pop = daily["pop"].as<float>() * 100;
    r.daily[i].precip = daily["rain"].as<float>() + daily["snow"].as<float>();
    JsonObject daily_weather = daily["weather"][0];
    r.daily[i].id = daily_weather["id"].as<int>();

    if (i == OWM_NUM_DAILY - 1)
    {
      break;
    }
    ++i;
  }

#if DISPLAY_ALERTS
  i = 0;
  for (JsonObject alerts : doc["alerts"].as<JsonArray>())
  {
    owm_alerts_t new_alert = {};
    new_alert.event = alerts["event"].as<const char *>();
    new_alert.tags = alerts["tags"][0].as<const char *>();
    r.alerts.push_back(new_alert);

    if (i == OWM_NUM_ALERTS - 1)
    {
      break;
    }
    ++i;
  }
#endif

  return error;
} // end deserializeMainCall

DeserializationError deserializeAirQuality(Stream &json,
                                           owm_resp_air_pollution_t &r)
{
  int i = 0;

  JsonDocument doc;

  DeserializationError error = deserializeJson(doc, json);
#if DEBUG_LEVEL >= 1
  Serial.println("[debug] doc.overflowed() : " + String(doc.overflowed()));
#endif
#if DEBUG_LEVEL >= 2
  serializeJsonPretty(doc, Serial);
#endif
  if (error)
  {
    return error;
  }

  for (JsonObject list : doc["list"].as<JsonArray>())
  {
    JsonObject list_components = list["components"];
    r.components.co[i] = list_components["co"].as<float>();
    r.components.no[i] = list_components["no"].as<float>();
    r.components.no2[i] = list_components["no2"].as<float>();
    r.components.o3[i] = list_components["o3"].as<float>();
    r.components.so2[i] = list_components["so2"].as<float>();
    r.components.pm2_5[i] = list_components["pm2_5"].as<float>();
    r.components.pm10[i] = list_components["pm10"].as<float>();
    r.components.nh3[i] = list_components["nh3"].as<float>();

    if (i == OWM_NUM_AIR_POLLUTION - 1)
    {
      break;
    }
    ++i;
  }

  return error;
} // end deserializeAirQuality

#endif