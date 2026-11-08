/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "Queue.h"
#include "main.h"
#include "cmsis_os.h"
#include "spi.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
TaskHandle_t CreateTaskHandle   = NULL;
TaskHandle_t PrintTask1Handle     = NULL;
TaskHandle_t PrintTask2Handle = NULL;
TaskHandle_t SpiSwapTaskHandle  = NULL;
QueueHandle_t DataQueueHandle = NULL;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void CreateTask(void *argument);
void PrintTask1(void *argument);
void PrintTask2(void *argument);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* 任务1：负责创建其他任务 */
  xTaskCreate(CreateTask, "CreateTask", 128, NULL, 1, &CreateTaskHandle);

  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/**
  * @brief  任务1：创建其他任务（任务2、任务3）
  * @param  argument: Not used
  * @retval None
  */
void CreateTask(void *argument)
{
  (void)argument;

	/* 先创建队列：消息大小 = sizeof(Message_t)，最多缓存 MSG_QUEUE_LENGTH 条 */
	DataQueueHandle = xQueueCreate(MSG_QUEUE_LENGTH, sizeof(Message_t));
	if (DataQueueHandle == NULL)
	{
		Error_Handler();
	}

	/* 任务1：spi任务（在队列创建之后启动，避免拿到空句柄）
	 * 注意：SpiTask 里是 HAL 忙等轮询，从不主动阻塞，
	 * 所以优先级必须低于产生消息的任务（PrintTask 为 2），
	 * 否则打印任务永远得不到 CPU，队列一直为空。 */
	xTaskCreate(SpiTask, "SpiTask", 512, NULL, 1, &SpiSwapTaskHandle);

  /* 任务2：打印任务1 */
  xTaskCreate(PrintTask1, "PrintTask1", 512, NULL, 2, &PrintTask1Handle);

  /* 任务3：打印任务2 */
  xTaskCreate(PrintTask2, "PrintTask2", 512, NULL, 2, &PrintTask2Handle);

  /* 创建完成后删除自身 */
  vTaskDelete(NULL);
}

/**
  * @brief  任务2：打印任务1
  * @param  argument: Not used
  * @retval None
  */
void PrintTask1(void *argument)
{
  (void)argument;

  /* 演示用的文本，实际项目里可以换成从串口/传感器得到的字符串 */
  const char *texts[] = {"Imtask1"};

  Message_t msg;
  const char *p;
  uint8_t   msg_id   = 0;
  uint8_t   text_idx = 0;
  uint8_t   n;

  for(;;)
  {
    /* 组一条消息：把文本拷进固定长度的 text 区 */
    p = texts[text_idx++ % (sizeof(texts) / sizeof(texts[0]))];
    n = (uint8_t)strlen(p);
    if (n > MSG_TEXT_LENGTH)
    {
      n = MSG_TEXT_LENGTH;
    }

    msg.head = MSG_HEAD;
    msg.id   = msg_id++;
    msg.len  = n;
    memset(msg.text, 0, sizeof(msg.text));
    memcpy(msg.text, p, n);

    /* 发到队列，队列满则丢弃本条，不阻塞 */
    if (xQueueSend(DataQueueHandle, &msg, 0) != pdPASS)
    {
      /* 可以在这里记录丢包 */
    }
		osDelay(500);
  }
}

/**
  * @brief  任务3：打印任务2
  * @param  argument: Not used
  * @retval None
  */
void PrintTask2(void *argument)
{
  (void)argument;
  const char *texts[] = {"ImTask2"};

  Message_t msg;
  const char *p;
  uint8_t   msg_id   = 0;
  uint8_t   text_idx = 0;
  uint8_t   n;
  for(;;)
  {
    /* 组一条消息：把文本拷进固定长度的 text 区 */
    p = texts[text_idx++ % (sizeof(texts) / sizeof(texts[0]))];
    n = (uint8_t)strlen(p);
    if (n > MSG_TEXT_LENGTH)
    {
      n = MSG_TEXT_LENGTH;
    }

    msg.head = MSG_HEAD;
    msg.id   = msg_id++;
    msg.len  = n;
    memset(msg.text, 0, sizeof(msg.text));
    memcpy(msg.text, p, n);

    /* 发到队列，队列满则丢弃本条，不阻塞 */
    if (xQueueSend(DataQueueHandle, &msg, 0) != pdPASS)
    {
      /* 可以在这里记录丢包 */
    }
		osDelay(500);
  }
}

/* USER CODE END Application */

