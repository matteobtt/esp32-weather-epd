#if WEATHER_API == OPEN_METEO

#include "weather_service.h"
#include "config.h"

#if HTTP_MODE == HTTPS_WITH_CERT_VERIF
#include "cert.h"

const char* TLS_CERT = cert_ISRG_Root_X1;
#endif

const String SERVICE_NAME = "Open Meteo API";

String buildURL() {
  return  "/v1/forecast?latitude=" + LAT + "&longitude=" + LON + "&" +
          "current=temperature_2m,relative_humidity_2m,dew_point_2m,apparent_temperature,weather_code,cloud_cover,visibility,surface_pressure,wind_speed_10m,wind_direction_10m,wind_gusts_10m,is_day&" +
          "hourly=temperature_2m,cloud_cover,wind_speed_10m,wind_gusts_10m,precipitation_probability,rain,snowfall,weather_code,is_day&" +
          "daily=weather_code,temperature_2m_max,temperature_2m_min,sunrise,sunset,uv_index_max,rain_sum,snowfall_sum,precipitation_probability_max,wind_speed_10m_max,wind_gusts_10m_max&" +
          "wind_speed_unit=ms&timezone=auto&timeformat=unixtime&forecast_days=5&forecast_hours=" + HOURLY_GRAPH_MAX;
}

String buildSanitizedURL(String url) {
  return OWM_ENDPOINT + url;
}

DeserializationError deserializeMainCall(WiFiClient &json,
                                              owm_resp_onecall_t &r)
{
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

  JsonObject current = doc["current"];
  JsonObject daily = doc["daily"];
  JsonObject hourly = doc["hourly"];

  r.current.dt = current["time"].as<int64_t>();
  r.current.sunrise = daily["sunrise"][0].as<int64_t>(); //
  r.current.sunset = daily["sunset"][0].as<int64_t>();   //
  r.current.temp = current["temperature_2m"].as<float>();
  r.current.feels_like = current["apparent_temperature"].as<float>();
  r.current.pressure = current["surface_pressure"].as<int>(); //
  r.current.humidity = current["relative_humidity_2m"].as<int>();
  r.current.dew_point = current["dew_point_2m"].as<float>();
  r.current.clouds = current["cloud_cover"].as<int>();
  r.current.uvi = daily["uv_index_max"][0].as<float>();     //
  r.current.visibility = current["visibility"].as<int>(); //
  r.current.wind_speed = current["wind_speed_10m"].as<float>();
  r.current.wind_gust = current["wind_gusts_10m"].as<float>();
  r.current.wind_deg = current["wind_direction_10m"].as<int>(); // w
  r.current.weather.id = current["weather_code"].as<int>();
  r.current.is_day = current["is_day"].as<bool>();

  int hours = doc["hourly"]["time"].size();
  for (size_t i = 0; i < hours; i++)
  {
    r.hourly[i].dt = hourly["time"][i].as<int64_t>(); // dt means
    r.hourly[i].temp = hourly["temperature_2m"][i].as<float>();
    r.hourly[i].clouds = hourly["cloud_cover"][i].as<int>();
    r.hourly[i].wind_speed = hourly["wind_speed_10m"][i].as<float>();
    r.hourly[i].wind_gust = hourly["wind_gusts_10m"][i].as<float>();
    r.hourly[i].pop = hourly["precipitation_probability"][i].as<int>();
    r.hourly[i].rain_1h = hourly["rain"][i].as<float>();
    r.hourly[i].snow_1h = hourly["snowfall"][i].as<float>();
    r.hourly[i].weather.id = hourly["weather_code"][i].as<int>();
    r.hourly[i].is_day = hourly["is_day"][i].as<bool>();

    if (i == OWM_NUM_HOURLY - 1)
    {
      break;
    }
  }

  int days = doc["daily"]["time"].size();
  for (size_t i = 0; i < days; i++)
  {
    r.daily[i].dt = daily["time"][i].as<int64_t>();
    // TODO: Open-Meteo does not provide lunar data. Calculate them or use another API.
    // r.daily[i].moonrise   = daily["moonrise"]  .as<int64_t>();
    // r.daily[i].moonset    = daily["moonset"]   .as<int64_t>();
    // r.daily[i].moon_phase = daily["moon_phase"].as<float>();
    r.daily[i].temp.min = daily["temperature_2m_min"][i].as<float>();
    r.daily[i].temp.max = daily["temperature_2m_max"][i].as<float>();
    // Cloud cover percentage is not provided by Open-Meteo as daily
    // r.daily[i].clouds = daily["cloud_cover"][i].as<int>();
    r.daily[i].wind_speed = daily["wind_speed_10m_max"][i].as<float>();
    r.daily[i].wind_gust = daily["wind_gusts_10m_max"][i].as<float>();
    r.daily[i].pop = daily["precipitation_probability_max"][i].as<int>();
    r.daily[i].rain = daily["rain_sum"][i].as<float>();
    r.daily[i].snow = daily["snowfall_sum"][i].as<float>();
    r.daily[i].weather.id = daily["weather_code"][i].as<int>();

    if (i == OWM_NUM_DAILY - 1)
    {
      break;
    }
  }

  // TODO: Open-Meteo does not issue alerts, use another API.
  /*
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
  */

  return error;
} // end deserializeOpenMeteoCall

#endif