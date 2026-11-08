/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
/* 队列长度 */
#define MSG_QUEUE_LENGTH    10U
/* 帧头，主机用它判断收到的是不是一帧新消息 */
#define MSG_HEAD            0xAAU
/* 文本区字节数（含可选的结尾 '\0'） */
#define MSG_TEXT_LENGTH     32U

/*
 * 通过队列传递、再经 SPI 发送的消息结构体。
 * 成员全是 1 字节类型：sizeof(Message_t) 固定为 35，没有结构体填充，
 * 可以直接按字节发送。SPI 主机侧必须使用完全相同的定义。
 * 文本按字节收发：ASCII 直接可用，中文请双方统一用 UTF-8。
 */
typedef struct
{
    uint8_t head;                   /* 帧头，固定 MSG_HEAD(0xAA) */
    uint8_t id;                     /* 消息编号 */
    uint8_t len;                    /* text 中有效字节数（不含结尾 '\0'） */
    char    text[MSG_TEXT_LENGTH];  /* 文本内容 */
} Message_t;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
