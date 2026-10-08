/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : FreeRTOS configuration
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "control.h"
#include "sbus.h"


/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


/* USER CODE END PTD */


/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define SBUS_SWA_CHANNEL       4//CH5

#define SBUS_LEFT_STICK        0

#define SBUS_RIGHT_STICK       2

#define SBUS_CENTER            1500

#define SWA_UP_THRESHOLD       1600//position 1

#define SWA_DOWN_THRESHOLD     1400//speed 2

#define STICK_DEAD_ZONE        30

#define MAX_ANGLE              90.0f

#define MAX_SPEED              2500.0f


/* USER CODE END PD */


/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */


/* USER CODE END PM */


/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

osThreadId RemoteTaskHandle;
osThreadId MotorTaskHandle;

volatile float target_angle = 0.0f;
volatile float target_speed = 0.0f;
volatile uint8_t control_mode = 0;

/* USER CODE END Variables */


/* Private function prototypes -----------------------------------------------*/

void StartRemoteTask(void const * argument);
void StartMotorTask(void const * argument);


/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(
    StaticTask_t **ppxIdleTaskTCBBuffer,
    StackType_t **ppxIdleTaskStackBuffer,
    uint32_t *pulIdleTaskStackSize
);


/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */

static StaticTask_t xIdleTaskTCBBuffer;

static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];


void vApplicationGetIdleTaskMemory(
    StaticTask_t **ppxIdleTaskTCBBuffer,
    StackType_t **ppxIdleTaskStackBuffer,
    uint32_t *pulIdleTaskStackSize
)
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;

    *ppxIdleTaskStackBuffer = &xIdleStack[0];

    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* USER CODE END GET_IDLE_TASK_MEMORY */


/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void)
{
    /* USER CODE BEGIN Init */

    /* USER CODE END Init */


    /* USER CODE BEGIN RTOS_MUTEX */

    /* USER CODE END RTOS_MUTEX */


    /* USER CODE BEGIN RTOS_SEMAPHORES */

    /* USER CODE END RTOS_SEMAPHORES */


    /* USER CODE BEGIN RTOS_TIMERS */

    /* USER CODE END RTOS_TIMERS */


    /* USER CODE BEGIN RTOS_QUEUES */

    /* USER CODE END RTOS_QUEUES */


    /* Create the thread(s) */

    osThreadDef(
        RemoteTask,
        StartRemoteTask,
        osPriorityNormal,
        0,
        128
    );

    RemoteTaskHandle =
        osThreadCreate(
            osThread(RemoteTask),
            NULL
        );


    osThreadDef(
        MotorTask,
        StartMotorTask,
        osPriorityAboveNormal,
        0,
        128
    );

    MotorTaskHandle =
        osThreadCreate(
            osThread(MotorTask),
            NULL
        );


    /* USER CODE BEGIN RTOS_THREADS */

    /* USER CODE END RTOS_THREADS */
}


/* USER CODE BEGIN Header_StartRemoteTask */
/**
  * @brief  Function implementing the RemoteTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartRemoteTask */

void StartRemoteTask(void const * argument)
{
    uint16_t swa_value;
    int16_t left_stick;
    int16_t right_stick;


    /* USER CODE BEGIN StartRemoteTask */

    for(;;)
    {
       
        swa_value = sbus_data.ch[SBUS_SWA_CHANNEL];

        left_stick =
            (int16_t)sbus_data.ch[SBUS_LEFT_STICK];

        right_stick =
            (int16_t)sbus_data.ch[SBUS_RIGHT_STICK];

        if(swa_value > SWA_UP_THRESHOLD)
        {
            control_mode = 1;

            if(
                left_stick > (SBUS_CENTER - STICK_DEAD_ZONE) &&
                left_stick < (SBUS_CENTER + STICK_DEAD_ZONE)
              )
            {
                
                target_angle = 0.0f;
            }
            else
            {
                target_angle =
                    (
                        (float)left_stick
                        - (float)SBUS_CENTER
                    )
                    / 500.0f
                    * MAX_ANGLE;
            }

            if(target_angle > MAX_ANGLE)
            {
                target_angle = MAX_ANGLE;
            }

            if(target_angle < -MAX_ANGLE)
            {
                target_angle = -MAX_ANGLE;
            }
						
            target_speed = 0.0f;
        }

        else if(swa_value < SWA_DOWN_THRESHOLD)
        {
            control_mode = 2;

            if(
                right_stick > (SBUS_CENTER - STICK_DEAD_ZONE) &&
                right_stick < (SBUS_CENTER + STICK_DEAD_ZONE)
              )
            {
               
                target_speed = 0.0f;
            }
            else
            {
                target_speed =
                    (
                        (float)right_stick
                        - (float)SBUS_CENTER
                    )
                    / 500.0f
                    * MAX_SPEED;
            }

            if(target_speed > MAX_SPEED)
            {
                target_speed = MAX_SPEED;
            }

            if(target_speed < -MAX_SPEED)
            {
                target_speed = -MAX_SPEED;
            }

            target_angle = 0.0f;
        }

        else
        {
            control_mode = 0;

            target_angle = 0.0f;

            target_speed = 0.0f;
        }
				
        osDelay(10);
    }

    /* USER CODE END StartRemoteTask */
}


/* USER CODE BEGIN Header_StartMotorTask */
/**
  * @brief Function implementing the MotorTask thread.
  * @param argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartMotorTask */

void StartMotorTask(void const * argument)
{
    /* USER CODE BEGIN StartMotorTask */

    for(;;)
    {
        if(control_mode == 1)
        {
           
            Motor_PositionControl(target_angle);
        }

        else if(control_mode == 2)
        {

            Motor_SpeedControl(target_speed);
        }

        else
        {
           
            Motor_SpeedControl(0.0f);
        }

        osDelay(10);
    }

    /* USER CODE END StartMotorTask */
}


/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */


/* USER CODE END Application */
