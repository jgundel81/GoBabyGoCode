
/*******************************************
 * 
 *    Go-Baby-Go Pink Jeep Source Code
 *  New England Insititute of Technology
 * 
 * Author: Joseph Gundel
 * 
 *      2/9/2019
 *  Name: Control.ino
 *  Description:
 *    Control Module 
 *    (Runs Steering and Driving Controls)
 *    Signals MotorControl Task
 * 
 *******************************************/

 
//Uncomment Below to disable Drive Motor
//#define DISABLE_DRIVE
//Uncomment Below to disable Steer Motor
//#define DISABLE_STEERING


//Steering Thresholds
#define LEFT_THRESHOLD_FAST 200
#define LEFT_THRESHOLD_SLOW 450
#define RIGHT_THRESHOLD_SLOW 650
#define RIGHT_THRESHOLD_FAST 900

//Driving Thresholds
#define FORWARD_THRESHOLD_FAST 200
#define FORWARD_THRESHOLD_SLOW 450
#define REVERSE_THRESHOLD_SLOW 650
#define REVERSE_THRESHOLD_FAST 950


//Flags for MotorControl Task signalling
bool gUpdateDriveFlag = false;
bool gUpdateSteerFlag = false;



typedef enum {
  FORWARD_FAST,
  FORWARD_SLOW,
  STOP,
  REVERSE_SLOW,
  REVERSE_FAST
} DRIVE_DIRECTION;


typedef enum {
  LEFT_FAST,
  LEFT_SLOW,
  STRAIGHT,
  RIGHT_SLOW,
  RIGHT_FAST
} STEERING_DIRECTION;


typedef struct _control {
  DRIVE_DIRECTION drive;
  STEERING_DIRECTION steer; 
} CONTROL;

static CONTROL controlStruct;
/*
 * Initialization Function for Control Module
 * Should only run once
 */
void initControl(void)
{
  static bool ranInit = false;
  if(true == ranInit)
    return;
    
  ranInit = true;
  controlStruct.drive = STOP;
  controlStruct.steer = STRAIGHT;
  Serial.println("Control Task Initialized"); 
}

#define WAIT_FOR_ROLLING_AVERAGE_TO_UPDATE 2000 
uint32_t controlTaskTimer = 0;

/*
 *   Function Name: controlTask(void) 
 * 
 *   Description: TASK-> controlTask
 *      Contains logic for Forward-Reverse and
 *      Left-Right Steering
 *     
 */
void controlTask(void)
{
   initControl(); // only runs once
   
   //Wait for Sensor Thread to Populate rolling Average (Only at Startup)
   if(millis() < WAIT_FOR_ROLLING_AVERAGE_TO_UPDATE)
      return;

    if(true == checkSteering())
    {
      printSteering();
#ifndef DISABLE_STEERING
      gUpdateSteerFlag = true;
#endif
    }
    if(true == checkDriving())
    {
      printDriving();
#ifndef DISABLE_DRIVE
      gUpdateDriveFlag = true;
#endif
    }
}


/*
 *   Function Name: checkSteering(void) 
 * 
 *   Description: Checks Steering Sensors
 *    Returns True if Steering has changed 
 *    and updates Steering in main Control
 *    Structure
 *     
 */
static bool checkSteering(void)
{
  bool retVal = false;
  uint32_t xVal = getXval();
  static CONTROL temp = {STOP , STRAIGHT};
  
  if(xVal <= LEFT_THRESHOLD_FAST)
  {
    controlStruct.steer = LEFT_FAST; 
  }
  else if(xVal <= LEFT_THRESHOLD_SLOW)
  {
    controlStruct.steer = LEFT_SLOW;
  }
  else if(xVal >= RIGHT_THRESHOLD_FAST)
  {
    controlStruct.steer = RIGHT_FAST;
  }
  else if(xVal >= RIGHT_THRESHOLD_SLOW)
  {
    controlStruct.steer = RIGHT_SLOW;
  }
  else
  {
    controlStruct.steer = STRAIGHT;
  }
  if(temp.steer != controlStruct.steer)
  {
    temp.steer = controlStruct.steer;
    retVal = true;
  }
  return retVal;
}


/*
 *   Function Name: checkDriving(void) 
 * 
 *   Description: Checks Driving Sensors
 *    Returns True if Driving has changed 
 *    and updates Driving in main Control
 *    Structure
 *     
 */
static bool checkDriving(void)
{
  bool retVal = false;
  uint32_t yVal = getYval();
  static CONTROL temp = {STOP , STRAIGHT};
  
  if(yVal <= FORWARD_THRESHOLD_FAST)
  {
    controlStruct.drive = FORWARD_FAST; 
  }
  else if(yVal <= FORWARD_THRESHOLD_SLOW)
  {
    controlStruct.drive = FORWARD_SLOW;
  }
  else if(yVal >= REVERSE_THRESHOLD_FAST)
  {
    controlStruct.drive = REVERSE_FAST;
  }
  else if(yVal >= REVERSE_THRESHOLD_SLOW)
  {
    controlStruct.drive = REVERSE_SLOW;
  }
  else
  {
    controlStruct.drive = STOP;
  }
  if(temp.drive != controlStruct.drive)
  {
    temp.drive = controlStruct.drive;
    retVal = true;
  }
  return retVal;
}



/*
 *   Function Name: printSteering(void) 
 * 
 *   Description: Prints Steering to
 *   Debug port
 *     
 */
void printSteering(void)
{
  switch(controlStruct.steer)
  {
    case LEFT_FAST:
      Serial.println("Left Fast");
     break;
    case LEFT_SLOW:
      Serial.println("Left Slow");
     break;
    case RIGHT_FAST:
      Serial.println("Right Fast");
     break;
    case RIGHT_SLOW:
      Serial.println("Right Slow");
     break;
    case STRAIGHT:
      Serial.println("Straight");
     break;
  }
}


/*
 *   Function Name: printDriving(void) 
 * 
 *   Description: Prints Driving to
 *   Debug port
 *     
 */
void printDriving(void)
{
  switch(controlStruct.drive)
  {
    case FORWARD_FAST:
      Serial.println("Forward Fast");
     break;
    case FORWARD_SLOW:
      Serial.println("Forward Slow");
     break;
    case REVERSE_FAST:
      Serial.println("Reverse Fast");
     break;
    case REVERSE_SLOW:
      Serial.println("Reverse Slow");
     break;
    case STOP:
      Serial.println("Stop");
     break;
  }
}

//*************** Public APIs ****************/
//Returns Direction Info(Left, Right, or Straight)
uint32_t getDirection()
{
  return controlStruct.steer;
}

//Returns Drive Info (Forward, Reverse, Stop)
uint32_t getDrive()
{
  return controlStruct.drive;
}
