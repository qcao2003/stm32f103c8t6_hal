/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */
#define USART_DMA_REC_SIE 256
#define USART_REC_SIE (USART_DMA_REC_SIE * 2)
typedef struct
{   
	uint8_t  UsartRecFlag;    // ���ڽ�����ɱ�־λ
	uint16_t UsartDMARecLen;  // ����DMA���ճ���
	uint16_t UsartRecLen;     // ���ڽ�����ɳ���
	uint8_t  UsartDMARecBuffer[USART_DMA_REC_SIE];  // ����DMA���ջ�����  
	uint8_t  UsartRecBuffer[USART_REC_SIE];         // ���ڽ��ջ�����
} ts_Usart_type;
extern ts_Usart_type Usart1type; 
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

