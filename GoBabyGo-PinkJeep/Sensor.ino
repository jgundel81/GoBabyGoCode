
/*******************************************
 * 
 *    Go-Baby-Go Pink Jeep Source Code
 *  New England Insititute of Technology
 * 
 * Author: Joseph Gundel
 * 
 *      2/9/2019
 *  Name: Sensor.ino
 *  Description: Sensor Module
 *    Samples sensors and calculates Rolling Average
 * 
 *******************************************/

//Uncomment below to printout RAW Sensor Readings to Debug port
//#define PRINT_RAW_SENSOR_VALUES 1

//Sensor Pins
const int JOYSTICK_XAXIS_PIN = A1;
const int JOYSTICK_YAXIS_PIN = A2;
const int CURRENTSENSOR_STEERING = A5; 
const int CURRENTSENSOR_DRIVING = A7;

#define SERIAL_PRINT_FREQUENCY 2000   //Printout readings every 2 Seconds

static uint32_t avgXval = 0;
static uint32_t avgYval = 0;

//Function Prototypes for Sensor Module
static uint32_t updateRollingAvgX(uint32_t rawData);
static uint32_t updateRollingAvgY(uint32_t rawData);


static uint32_t sensorTaskTimer = 0; //Hold timer for Sensor Tasks

/*
 * Initialization Function for Sensor Module
 * Should only run once
 */
void initSensors(void)
{
  static bool ranInit = false;
  if(true == ranInit)
    return;
    
  Serial.println("Serial Task Running");
  ranInit = true;
}



/*
 *   Function Name: sensorTask(void) 
 * 
 *   Description: TASK-> Main Sensor Task
 *   Handles Sampling and Averaging Sensors
 *     
 */ 
void sensorTask(void)
{
  initSensors(); // Only Runs once

  //Sample X Axis (Left-Right)
  uint32_t xAxisRaw = analogRead(JOYSTICK_XAXIS_PIN);
  avgXval = updateRollingAvgX(xAxisRaw);

  //Sample Y Axis (Forward-Reverse)
  uint32_t yAxisRaw = analogRead(JOYSTICK_YAXIS_PIN);
  avgYval = updateRollingAvgY(yAxisRaw);

  //Sample Current Sensors
  uint32_t steeringCurrentRaw = analogRead(CURRENTSENSOR_STEERING);
  uint32_t drivingCurrentRaw = analogRead(CURRENTSENSOR_DRIVING);


#ifdef PRINT_RAW_SENSOR_VALUES
  if((millis() - sensorTaskTimer) > SERIAL_PRINT_FREQUENCY)
  {
    Serial.print(" Xaxis ->");
    Serial.println(xAxisRaw);
    Serial.print(" Yaxis ->");
    Serial.println(yAxisRaw);
    Serial.print(" Steering Current ->");
    Serial.println(steeringCurrentRaw);
    Serial.print(" Driving Current ->");
    Serial.println(drivingCurrentRaw);
    Serial.println();
    Serial.println();
    Serial.println();    
    sensorTaskTimer = millis();
  }
#endif
  
}

/*
 *   Function Name: updateRollingAvgX(uint32_t rawData) 
 * 
 *   Description: updates and calculates rolling avg
 *   for X-Axis
 *     
 */ 
static uint32_t updateRollingAvgX(uint32_t rawData)
{
  static uint32_t RawVal[8] = {0};
  static uint32_t avg = 0;
  for(int i = 0; i <7; i++)
  {
    RawVal[i] = RawVal[i + 1];
  }
  RawVal[7] = rawData;
  for(int i = 0; i < 8; i++)
  {
    avg = avg + RawVal[i];
  }
  avg = avg / 8;
  return avg;
}


/*
 *   Function Name: updateRollingAvgY(uint32_t rawData) 
 * 
 *   Description: updates and calculates rolling avg
 *   for Y-Axis
 *     
 */ 
static uint32_t updateRollingAvgY(uint32_t rawData)
{
  static uint32_t RawVal[8] = {0};
  static uint32_t avg = 0;
  for(int i = 0; i <7; i++)
  {
    RawVal[i] = RawVal[i + 1];
  }
  RawVal[7] = rawData;
  for(int i = 0; i < 8; i++)
  {
    avg = avg + RawVal[i];
  }
  avg = avg / 8;
  return avg;
}


//Public API
uint32_t getXval() 
{
  return avgXval;
}
uint32_t getYval()
{
  return avgYval;
}
