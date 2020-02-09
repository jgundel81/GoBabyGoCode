
/*******************************************
 * 
 *    Go-Baby-Go Pink Jeep Source Code
 *  New England Insititute of Technology
 * 
 * Author: Joseph Gundel
 * 
 *      2/9/2019
 *  Name: GoBabyGo-PinkJeep.ino
 *  Description:
 *    MotorControl Module (Controls Motors)
 *    Receives Control Task Flags
 * 
 *******************************************/

//Pin Assignements 
const int DRIVE_MOTOR_L_EN = 2;
const int DRIVE_MOTOR_R_EN = 4;
const int DRIVE_MOTOR_L_PWM = 3;
const int DRIVE_MOTOR_R_PWM = 5;

const int STEER_MOTOR_L_EN = 6;
const int STEER_MOTOR_R_EN = 8;
const int STEER_MOTOR_L_PWM = 7;
const int STEER_MOTOR_R_PWM = 9;


//Default PWM Speeds (0-255)

#define DRIVE_PWM_SLOW 80
#define DRIVE_PWM_FAST 150
#define STEER_PWM_FAST 100
#define STEER_PWM_SLOW 200


/*
 *   Function Name: initMotor(void) 
 * 
 *   Description: intializes Motor Task
 *   Configure Motor IO Pins
 *     
 */
void initMotor(void)
{
  static bool ranInit = false;
  if(true == ranInit)
    return;
    
  ranInit = true;
   
  pinMode(DRIVE_MOTOR_L_EN,OUTPUT);
  pinMode(DRIVE_MOTOR_R_EN,OUTPUT);
  pinMode(DRIVE_MOTOR_L_PWM,OUTPUT);
  pinMode(DRIVE_MOTOR_R_PWM,OUTPUT);
  
  pinMode(STEER_MOTOR_L_EN,OUTPUT);
  pinMode(STEER_MOTOR_R_EN,OUTPUT);
  pinMode(STEER_MOTOR_L_PWM,OUTPUT);
  pinMode(STEER_MOTOR_R_PWM,OUTPUT); 
  Stop();
  Serial.println("Motor Task Initialized");
 
}



/*
 *   Function Name: motorTask(void) 
 * 
 *   Description: MotorTask is responsible
 *   for controlling Motors
 *   Receives Flags from Control Task
 *     
 */
void motorTask(void)
{
  initMotor();
  if(true == gUpdateDriveFlag)
  {
    gUpdateDriveFlag = false;
    switch(getDrive())
    {
      case FORWARD_FAST:
        driveForward(true);
      break;
      case FORWARD_SLOW:
        driveForward(false);
      break;
      case REVERSE_FAST:
        driveReverse(true);
      break;
      case REVERSE_SLOW:
        driveReverse(false);
      break;
      case STOP:
        Stop();
      break;
    }
  }
  if(true == gUpdateSteerFlag)
  {
    gUpdateDriveFlag = false;
    switch(getDirection())
    {
      case LEFT_FAST:
        driveLeft(true);
      break;
      case LEFT_SLOW:
        driveLeft(false);
      break;
      case RIGHT_FAST:
        driveRight(true);
      break;
      case RIGHT_SLOW:
        driveRight(false);
      break;
      case STRAIGHT:
        Straight();
      break;
    }
  }
}


/*
 *   Function Name: Stop(void) 
 * 
 *   Description: Sets Motor Control
 *   pins LOW (STOPS Motors) 
 *     
 */
static void Stop(void)
{
  digitalWrite(DRIVE_MOTOR_L_PWM, LOW);
  digitalWrite(DRIVE_MOTOR_R_PWM, LOW);
  digitalWrite(DRIVE_MOTOR_L_EN, LOW);
  digitalWrite(DRIVE_MOTOR_R_EN, LOW);
}

/*
 *   Function Name: driveReverse(bool fast) 
 * 
 *   Description: Sets Rear Motor to Drive 
 *   in Reverse. If fast is true will drive
 *   Fast Speed.
 *     
 */
static void driveReverse(bool fast)
{
  if(true == fast)
  {
    analogWrite(DRIVE_MOTOR_R_PWM, DRIVE_PWM_FAST);
    digitalWrite(DRIVE_MOTOR_L_PWM, LOW);
    digitalWrite(DRIVE_MOTOR_L_EN, HIGH);
    digitalWrite(DRIVE_MOTOR_R_EN, HIGH);
  }
  else
  {
    analogWrite(DRIVE_MOTOR_R_PWM, DRIVE_PWM_SLOW);
    digitalWrite(DRIVE_MOTOR_L_PWM, LOW);
    digitalWrite(DRIVE_MOTOR_L_EN, HIGH);
    digitalWrite(DRIVE_MOTOR_R_EN, HIGH);
  }
}


/*
 *   Function Name: driveForward(bool fast) 
 * 
 *   Description: Sets Rear Motor to Drive 
 *   Forward. If fast is true will drive
 *   Fast Speed.
 *     
 */
static void driveForward(bool fast)
{
  if(true == fast)
  {
    digitalWrite(DRIVE_MOTOR_R_PWM, LOW);
    analogWrite(DRIVE_MOTOR_L_PWM, DRIVE_PWM_FAST);
    digitalWrite(DRIVE_MOTOR_L_EN, HIGH);
    digitalWrite(DRIVE_MOTOR_R_EN, HIGH);
  }
  else
  {
    digitalWrite(DRIVE_MOTOR_R_PWM, LOW);
    analogWrite(DRIVE_MOTOR_L_PWM, DRIVE_PWM_SLOW);
    digitalWrite(DRIVE_MOTOR_L_EN, HIGH);
    digitalWrite(DRIVE_MOTOR_R_EN, HIGH);
  }
}


/*
 *   Function Name: Straignt(void) 
 * 
 *   Description: Sets Steering Motor Off 
 *     
 */
static void Straight(void)
{
  digitalWrite(STEER_MOTOR_L_PWM, LOW);
  digitalWrite(STEER_MOTOR_R_PWM, LOW);
  digitalWrite(STEER_MOTOR_L_EN, LOW);
  digitalWrite(STEER_MOTOR_R_EN, LOW);
}


/*
 *   Function Name: driveLeft(bool fast) 
 * 
 *   Description: Sets Steering Motor to Drive 
 *   Left. If fast is true will drive
 *   Fast Speed.
 *     
 */
static void driveLeft(bool fast)
{
  if(true == fast)
  {
    analogWrite(STEER_MOTOR_R_PWM, STEER_PWM_FAST);
    digitalWrite(STEER_MOTOR_L_PWM, LOW);
    digitalWrite(STEER_MOTOR_L_EN, HIGH);
    digitalWrite(STEER_MOTOR_R_EN, HIGH);
  }
  else
  {
    analogWrite(STEER_MOTOR_R_PWM, STEER_PWM_SLOW);
    digitalWrite(STEER_MOTOR_L_PWM, LOW);
    digitalWrite(STEER_MOTOR_L_EN, HIGH);
    digitalWrite(STEER_MOTOR_R_EN, HIGH);
  }
}


/*
 *   Function Name: driveRight(bool fast) 
 * 
 *   Description: Sets Steering Motor to Drive 
 *   Right. If fast is true will drive
 *   Fast Speed.
 *     
 */
static void driveRight(bool fast)
{
  if(true == fast)
  {
    digitalWrite(STEER_MOTOR_R_PWM, LOW);
    analogWrite(STEER_MOTOR_L_PWM, STEER_PWM_FAST);
    digitalWrite(STEER_MOTOR_L_EN, HIGH);
    digitalWrite(STEER_MOTOR_R_EN, HIGH);
  }
  else
  {
    digitalWrite(STEER_MOTOR_R_PWM, LOW);
    analogWrite(STEER_MOTOR_L_PWM, STEER_PWM_SLOW);
    digitalWrite(STEER_MOTOR_L_EN, HIGH);
    digitalWrite(STEER_MOTOR_R_EN, HIGH);
  }
}
