/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "SDdriver.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//void WritetoSD(BYTE write_buff[],uint8_t bufSize);
char SD_FileName[] = "hello.txt";
uint8_t WriteBuffer[] = "01 窝室嫩叠\r\n";
uint8_t ReadBuffer[10];

//uint8_t test_sd =0;	//用于测试格式化
uint8_t write_cnt =0;	//写SD卡次数


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
//	const char message[] = "hello windows!\r\n";  //定义数据发送数组
	char Data[50],Time[50],Weekday[50];
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_RTC_Init();
  MX_SPI1_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
	OLED_Init();
	OLED_Clear();
	//OLED_ShowString(0,0,"hello",6);
	//OLED_ShowNum(5,0,249,4,16);

	// 开启串口接收中断
  HAL_UART_Receive_IT(&huart1, (uint8_t *)&usart1Recv.recvData, 1);

	Get_SDCard_Capacity();	//得到使用内存并选择格式化
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		MyRTC_ReadTime();

		sprintf(Data,"%d%s%d%s%d",MyRTC_Time[0],"/", MyRTC_Time[1],"/",MyRTC_Time[2]);
		sprintf(Time,"%s%d%s%d%s%d","Time is ",MyRTC_Time[3],":",MyRTC_Time[4],":",MyRTC_Time[5]);
		//sprintf(Weekday,"%s%s","Today's ", Weekdays);
		OLED_ShowString(0,0,(uint8_t*)Data,16);
		OLED_ShowString(0,2,(uint8_t*)Time,16);
		//OLED_ShowString(0,4,(uint8_t*)Weekday,16);
		if(MyRTC_Time[5] == 59) //当秒是59的时候就刷新
			OLED_Clear();
		if(usart1Recv.recvFlag == 1)
		{
			usart1Recv.recvFlag = 0;
			if(strcmp(usart1Recv.recvBuff, "LED\r") == 0)
			{
				// 发送数据
				printf(usart1Recv.recvBuff);
				printf("temp is %d\r\n",temp);
				printf("RES_OK is %d\r\n", RES_OK);
				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
				
			}
			if(strcmp(usart1Recv.recvBuff, "Write\r") == 0)
			{
				// 写入SD卡
				WritetoSD(WriteBuffer,sizeof(WriteBuffer),SD_FileName);	
				WriteBuffer[0] = WriteBuffer[0] +0;
				WriteBuffer[1] = WriteBuffer[1] +1;
				write_cnt ++;
				ReadtoSD(ReadBuffer,5,"data1.txt");
				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
				OLED_ShowString(0,4,(uint8_t*)ReadBuffer,16);
			}
			// 发送数据
			printf("UART_FLAG\r\n");
			memset(usart1Recv.recvBuff,0,sizeof(usart1Recv.recvBuff));
		}

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
