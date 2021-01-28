/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "WS2812B.h"
#include "stdio.h"

extern uint8_t LED_MODE;
//extern uint8_t R;
//extern uint8_t G;
//extern uint8_t B;
uint8_t Usart1_Data[1];
uint8_t Usart2_Data[1];
uint8_t RGB_color[3] = {0,0,0};

uint8_t AT[] = "AT\r\n";
uint8_t AT_CWJAP_DEF[] = "AT+CWJAP_DEF=\"JCIL RD-Test\",\"test12345\"\r\n";
uint8_t AT_CIPSTART[] ="AT+CIPSTART=\"TCP\",\"192.168.1.10\",5000\r\n";

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

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    uint8_t color_black[] = {0, 0, 0};
    uint8_t color_white[] = {255,255,255};    
    uint8_t color_red[] = {255, 0, 0};
    uint8_t color_yellow[] = {255,255,0};
    uint8_t color_green[] = {0, 255, 0};
    uint8_t color_cyan[] = {0,255,255};
    uint8_t color_blue[] = {0, 0, 255};
    uint8_t color_pink[] = {255,0,255};
    uint8_t color_orange[] = {255,32,0};
    uint8_t color_purple[] = {128,0,128};
    
    uint8_t *color[] = {color_red,color_yellow,color_green,color_cyan,color_blue,color_pink,color_orange,color_purple};
    
/*
    uint16_t buff[28+24+24] = {0,0,0,
                        0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,
                        0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,
                        0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,
                        0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,
                        0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,
                        0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,
                        0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,
                        0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,
                        0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,
                        0};
    */
    
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
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_SPI3_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

    printf("WS2812B_TEST\r\n");

    WS2812_send(color_black, 60);
    HAL_Delay(SPEED);
    
    HAL_UART_Receive_IT(&huart1,Usart1_Data,1);
    HAL_UART_Receive_IT(&huart2,Usart2_Data,1);
    
    while(HAL_UART_Transmit(&huart2, (uint8_t*)AT, sizeof(AT), 5000)!= HAL_OK);
    HAL_Delay(3000);
    while(HAL_UART_Transmit(&huart2, (uint8_t*)AT, sizeof(AT), 5000)!= HAL_OK);
    HAL_Delay(3000);
    while(HAL_UART_Transmit(&huart2, (uint8_t*)AT, sizeof(AT), 5000)!= HAL_OK);
    HAL_Delay(3000);

    while(HAL_UART_Transmit(&huart2, (uint8_t*)AT_CWJAP_DEF, sizeof(AT_CWJAP_DEF), 5000)!= HAL_OK);
    HAL_Delay(10000);
    
    while(HAL_UART_Transmit(&huart2, AT_CIPSTART, sizeof(AT_CIPSTART), 5000)!= HAL_OK);
    
    
    

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      

        HAL_Delay(1000);



      switch(LED_MODE)
      {
          case 0x00:
            WS2812_send(RGB_color, LED_NUM);
            break;
          
          case 1:WS2812_Wave_light(RGB_color,LED_NUM);break;
          case 2:WS2812_Breathing_light(RGB_color,LED_NUM);break;
          case 3:WS2812_Double_Color_Multistage_Waterfall_light(RGB_color,color_white,LED_NUM);break;
          
          case 4:WS2812_Rainbow_Flow(LED_NUM);break;
          case 5:WS2812_RGB_GradualChange_Waterfall(LED_NUM);break;
                 
          case 6:WS2812_Colorful_Jump_light(color,sizeof(color)/sizeof(uint8_t *),LED_NUM);break;
          
          case 7:WS2812_Colourful_Waterfall_light(color,sizeof(color)/sizeof(uint8_t *),LED_NUM);break;
          
          
          
      }

        
      
        
    
      HAL_GPIO_TogglePin(D2_GPIO_Port,D2_Pin);
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
