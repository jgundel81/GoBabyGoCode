
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
 *    Main Module (Runs Setup and Scheduler)
 * 
 *******************************************/
const int LED_PIN = 13;


//Common shared function prototypes
uint32_t getXval();
uint32_t getYval();


/********************* Tasks *************************/
void blinkLED(void);
void sensorTask(void);
void controlTask(void);
void motorTask(void);


/********************* Scheduling Related Variables *************************/

#define INTERVAL_ALWAYS 0
#define INTERVAL_100ms 100
#define INTERVAL_500ms 500
#define INTERVAL_1000ms 1000

typedef struct _task
{
  uint16_t interval;
  uint32_t lastTick;
  void (*func)(void); 
} TaskType;


static TaskType *pTask = NULL;
static uint8_t taskIndex = 0;



/*********    TASk Table (insert Tasks into Table **********************/
static TaskType Tasks[] = {
  {INTERVAL_1000ms, 0, blinkLED},
  {INTERVAL_100ms,  0, sensorTask},
  {INTERVAL_ALWAYS,  0, controlTask},
  {INTERVAL_ALWAYS,  0, motorTask},
  
};

const uint8_t numOfTasks = sizeof(Tasks) / sizeof(*Tasks);

TaskType *getTable(void)
{
  return Tasks;
}
    

/*
 *   Function Name: Setup 
 * 
 *   Description: Setup function, Initialized LED, Serial port and
 *     task scheduler structures
 */
void setup() 
{
   pinMode(LED_PIN,OUTPUT);
   Serial.begin(9600);
   pTask = getTable();
   if(NULL == pTask)
   {
      //Error
      while(1);
   }
}


/*
 *   Function Name: Loop 
 * 
 *   Description: Scheduler- Calls tasks at stated intervals
 *     
 */
void loop() 
{
  for(taskIndex = 0; taskIndex <numOfTasks; taskIndex++)
  {
     //Run primitive Scheduler
     if(0 == pTask[taskIndex].interval)
     {
        //run every loop
        (*pTask[taskIndex].func)();
     }
     else if((millis() - pTask[taskIndex].lastTick) > pTask[taskIndex].interval)
     {
        (*pTask[taskIndex].func)();    
        pTask[taskIndex].lastTick = millis();       
     }
  }
}


/*
 *   Function Name: blinkLED 
 * 
 *   Description: Blinks LED 1000ms
 *   
 */
void blinkLED(void)
{
  static bool ledState = false;
  digitalWrite(LED_PIN, ledState);
  if(ledState == true)
    ledState=false;
  else
    ledState = true;    
}
