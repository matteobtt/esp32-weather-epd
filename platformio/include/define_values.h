// E-PAPER PANEL
#define DISP_BW_V2  1
#define DISP_3C_B   2
#define DISP_7C_F   3
#define DISP_BW_V1  4

// E-PAPER DRIVER BOARD
#define DRIVER_DESPI_C02 1
#define DRIVER_WAVESHARE 2

// INDOOR ENVIRONMENT SENSOR
#define BME280 1
#define BME680 2

// UNITS - TEMPERATURE
#define KELVIN       1
#define CELSIUS      2
#define FAHRENHEIT   3

// UNITS - WIND SPEED
#define METERSPERSECOND     1
#define FEETPERSECOND       2
#define KILOMETERSPERHOUR   3
#define MILESPERHOUR        4
#define KNOTS               5
#define BEAUFORT            6

// UNITS - PRESSURE
#define HECTOPASCALS                1
#define PASCALS                     2
#define MILLIMETERSOFMERCURY        3
#define INCHESOFMERCURY             4
#define MILLIBARS                   5
#define ATMOSPHERES                 6
#define GRAMSPERSQUARECENTIMETER    7
#define POUNDSPERSQUAREINCH         8

// UNITS - VISIBILITY DISTANCE
#define KILOMETERS  1
#define MILES       2

// UNITS - PRECIPITATION
#define POP         1
#define MILLIMETERS 2
#define CENTIMETERS 3
#define INCHES      4

// Hypertext Transfer Protocol (HTTP)
#define HTTP                    1
#define HTTPS_NO_CERT_VERIF     2
#define HTTPS_WITH_CERT_VERIF   3  // REQUIRES MANUAL UPDATE WHEN CERT EXPIRES

// WIND DIRECTION ICON PRECISION AND WIND DIRECTION LABEL
#define NUMBER                  1  // Only for label
#define CARDINAL                2
#define INTERCARDINAL           3
#define SECONDARY_INTERCARDINAL 4
#define TERTIARY_INTERCARDINAL  5
#define ANY_360                 6  // Only for arrow