#include "hardware_manager.h"
#include "movingAvg.h"

/**
  * @brief Calculate altitude difference between a ground pressure and a current pressure.
  * @param pres current pressure, atmospheres
  * @param pressureReference reference pressure (i.e. at ground level)
  * @return Altitude above reference level in meters
*/
float pressure_to_meters(float pres, float pressureReference) {
  double lapseRate = -0.0065;    // valid below 11000m
  double tempRef = 20 + 273.15;  // C to K
  double rDryAir = 287.0474909;  // J/K/kg
  double gAccelEarth = 9.80665;  // m/s**2
  return (tempRef / lapseRate) * (pow(pres / pressureReference, -rDryAir * lapseRate / gAccelEarth) - 1);
}

// Creates an instance of the HardwareManager

const int delay_period_ms = 16;
int last_time_ms = millis();

HardwareManager hardware_manager;

const int launch_accel_threshold = 30;  // meters per second squared
const int launch_accel_time = 300;      // ms required being over launch_accel_threshold before being sure
uint32_t accel_threshold_time;
float current_altitude_meters = 0.0;
float last_altitude_meters = 0.0;
float pressure_reference = 1.0;
movingAvg altitude_avg_filter(30);
float last_moving_average_altitude = 0;
float current_moving_average_altitude = 0;


enum STATE {
  PREFLIGHT,
  ASCENT,
  DESCENT,
  POSTFLIGHT,
  NUM_STATES,
};

int state;
int state_count;



void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  delay(3000);

  // Initialize the hardware manager here:
  hardware_manager.init();

  state = PREFLIGHT;
  state_count = 0;
  accel_threshold_time = millis();
  altitude_avg_filter.begin();
}

void loop() {
  if (millis() - last_time_ms < delay_period_ms) {
    return;
  }
  last_time_ms = millis();

  hardware_manager.readSensorData();
  hardware_manager.logDataPacket();
  SensorData_t data = hardware_manager.getSensorData();

  last_altitude_meters = current_altitude_meters;
  current_altitude_meters = pressure_to_meters(data.pressure, pressure_reference);

  altitude_avg_filter.reading(current_altitude_meters * 1000);

  last_moving_average_altitude = current_moving_average_altitude;
  current_moving_average_altitude = altitude_avg_filter.getAvg();


  // Serial.print("pressure:");
  // Serial.print(data.pressure);
  // Serial.print(",");
  // Serial.print("altitude_meters:");
  // Serial.print(current_altitude_meters);
  // Serial.print(",");
  // Serial.print("accel:");
  // Serial.println(data.accel_x);


  switch (state) {
    case PREFLIGHT:
      // update ground level pressure reference
      pressure_reference = data.pressure;

      //Serial.println("Preflight");

      if (!(data.accel_x > launch_accel_threshold)) {
        accel_threshold_time = millis();
      }

      if (millis() - accel_threshold_time > launch_accel_time) {
        state = ASCENT;
      }

      break;
    case ASCENT:
      Serial.println("Ascent");
      if (current_moving_average_altitude / 1000.0 < last_moving_average_altitude / 1000.0) {
        state = DESCENT;
      }
      break;
    case DESCENT:
      Serial.println("Descent");
      break;
    case POSTFLIGHT:
      // Serial.println("Postflight");
      break;
    default:
      break;
  }

  state_count++;
}
