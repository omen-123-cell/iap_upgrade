/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
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
#include "spi.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
/* USER CODE BEGIN 0 */
#include "queue.h"

/* 从机等待主机时钟的超时时间(ms) */
#define SPI_SWAP_TIMEOUT    1000U
/* 发送一条消息时等待主机时钟的超时时间(ms) */
#define SPI_MSG_TIMEOUT     5000U

/* 队列句柄定义在 freertos.c 中 */
extern QueueHandle_t DataQueueHandle;

/* USER CODE END 0 */

SPI_HandleTypeDef hspi1;

/* SPI1 init function */
void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_SLAVE;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspInit 0 */

  /* USER CODE END SPI1_MspInit 0 */
    /* SPI1 clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PA4     ------> SPI1_NSS
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP ;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


  /* USER CODE BEGIN SPI1_MspInit 1 */

  /* USER CODE END SPI1_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspDeInit 0 */

  /* USER CODE END SPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();

    /**SPI1 GPIO Configuration
    PA4     ------> SPI1_NSS
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

  /* USER CODE BEGIN SPI1_MspDeInit 1 */

  /* USER CODE END SPI1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/**
  * @brief  SPI1 从机交换一个字节（发一个字节的同时收一个字节）
  * @param  TxData: 要发送的数据
  * @retval 接收到的数据；超时/出错返回 0x00
  * @note   SPI1 配置为从机：PA4-NSS, PA5-SCK, PA6-MISO, PA7-MOSI。
  *         每调用一次需要主机提供 8 个时钟，等待超过
  *         SPI_SWAP_TIMEOUT 毫秒则返回 0x00
  */


void SpiTask(void *argument)
{
    Message_t tx_msg = {0};
    Message_t rx_msg;
    tx_msg.head = 0x00;                 /* 没消息时发空帧, 主机会丢弃 */

    for (;;)
    {
        Message_t new_msg;

        /* 不阻塞: 有消息就更新要发的帧, 没消息继续发上一帧 */
        if (xQueueReceive(DataQueueHandle, &new_msg, 0) == pdTRUE)
        {
            if (new_msg.len > sizeof(new_msg.text))
                new_msg.len = sizeof(new_msg.text);
            new_msg.head = 0xAA;
            tx_msg = new_msg;
        }

        /* 始终等在 SPI 上, 保证主机每次拉时钟时从机已经就绪 */
        if(HAL_SPI_TransmitReceive(&hspi1,(uint8_t *)&tx_msg,(uint8_t *)&rx_msg,(uint16_t)sizeof(Message_t),SPI_MSG_TIMEOUT) == HAL_OK)
          HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,GPIO_PIN_SET);
				else
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,GPIO_PIN_RESET);
                                          





    
    }
}


/* USER CODE END 1 */

