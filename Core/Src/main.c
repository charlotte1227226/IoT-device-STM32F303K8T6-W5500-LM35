/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "adc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "w5500.h"
#include "w5500_spi.h"
#include "w5500_socket.h"
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
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  // W5500 初始化
  SPI_Delay(10); // 等待 SPI 初始化穩定
  W5500_DevTypeDef w5500_dev;
  W5500_StatusTypeDef status = W5500_Init(SPI1_ID, &w5500_dev);
  SPI_Delay(10); // 等待 W5500 初始化穩定
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if (status != W5500_OK) {
    printf("W5500 initialization failed\n");
    }
    printf("W5500 initialized successfully\n");
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    uint16_t raw = HAL_ADC_GetValue(&hadc1);
    float voltage = ((float)raw / 4095.0) * 3.3; // 單位：V
    float temperature_C = voltage * 10.0; // LM35 每 10mV = 1°C
    printf("ADC raw: %u, voltage: %.2f V, temperature: %.2f °C\r\n", raw, voltage, temperature_C);
    HAL_Delay(500);
    printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", w5500_dev.MAC[0], w5500_dev.MAC[1], w5500_dev.MAC[2], w5500_dev.MAC[3], w5500_dev.MAC[4], w5500_dev.MAC[5]);
    SPI_Delay(10);
    printf("IP: %d.%d.%d.%d\n", w5500_dev.IP[0], w5500_dev.IP[1], w5500_dev.IP[2], w5500_dev.IP[3]);
    SPI_Delay(10);
    printf("Subnet: %d.%d.%d.%d\n", w5500_dev.SUBNET[0], w5500_dev.SUBNET[1], w5500_dev.SUBNET[2], w5500_dev.SUBNET[3]);
    SPI_Delay(10);
    printf("Gateway: %d.%d.%d.%d\n", w5500_dev.GATEWAY[0], w5500_dev.GATEWAY[1], w5500_dev.GATEWAY[2], w5500_dev.GATEWAY[3]);
    SPI_Delay(10);
    printf("Retry Time: %d\n", w5500_dev.RetryTime);
    SPI_Delay(10);
    printf("Retry Count: %d\n", w5500_dev.RetryCount);
    SPI_Delay(10);
    printf("PHYCFGR: %02X\n", w5500_dev.PHYCFGR);
    SPI_Delay(10);
    printf("Version: %02X\n", w5500_dev.Version);
    SPI_Delay(10);
    uint32_t cr1 = hspi1.Instance->CR1;
    // bit1 = CPOL, bit0 = CPHA
    printf("SPI1 CR1 = 0x%08lX (CPOL=%d, CPHA=%d)\n", cr1, (cr1 & SPI_CR1_CPOL)>>1, (cr1 & SPI_CR1_CPHA)>>0);    // // 測試寫入 W5500_GAR0
    W5500_DevTypeDef w5500_dev_test;
    W5500_StatusTypeDef test_status = W5500_Read_Init(SPI1_ID, &w5500_dev_test);
    printf("read ------\n");
    printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", w5500_dev_test.MAC[0], w5500_dev_test.MAC[1], w5500_dev_test.MAC[2], w5500_dev_test.MAC[3], w5500_dev_test.MAC[4], w5500_dev_test.MAC[5]);
    SPI_Delay(10);
    printf("IP: %d.%d.%d.%d\n", w5500_dev_test.IP[0], w5500_dev_test.IP[1], w5500_dev_test.IP[2], w5500_dev_test.IP[3]);
    SPI_Delay(10);
    printf("Subnet: %d.%d.%d.%d\n", w5500_dev_test.SUBNET[0], w5500_dev_test.SUBNET[1], w5500_dev_test.SUBNET[2], w5500_dev_test.SUBNET[3]);
    SPI_Delay(10);
    printf("Gateway: %d.%d.%d.%d\n", w5500_dev_test.GATEWAY[0], w5500_dev_test.GATEWAY[1], w5500_dev_test.GATEWAY[2], w5500_dev_test.GATEWAY[3]);
    SPI_Delay(10);
    printf("Retry Time: %d\n", w5500_dev_test.RetryTime);
    SPI_Delay(10);
    printf("Retry Count: %d\n", w5500_dev_test.RetryCount);
    SPI_Delay(10);
    printf("PHYCFGR: %02X\n", w5500_dev_test.PHYCFGR);
    SPI_Delay(10);
    printf("Version: %02X\n", w5500_dev_test.Version);
    SPI_Delay(10);
    uint8_t phycfgr;
    W5500_Read_Byte(SPI1_ID, W5500_BSB_COMMON, W5500_PHYCFGR, &phycfgr);
    printf("PHYCFGR: 0x%02X → LNK=%d SPD=%d DPX=%d\n", phycfgr, phycfgr&0x01, (phycfgr>>1)&0x01, (phycfgr>>2)&0x01);
    uint8_t IR, IMR, SIMR, SIR;
    W5500_Read_Byte(SPI1_ID, W5500_BSB_COMMON, W5500_IR, &IR);
    W5500_Read_Byte(SPI1_ID, W5500_BSB_COMMON, W5500_IMR, &IMR);
    W5500_Read_Byte(SPI1_ID, W5500_BSB_COMMON, W5500_SIMR, &SIMR);
    W5500_Read_Byte(SPI1_ID, W5500_BSB_COMMON, W5500_SIR, &SIR);
    printf("IR: 0x%02X, IMR: 0x%02X, SIMR: 0x%02X, SIR: 0x%02X\n", IR, IMR, SIMR, SIR);

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
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
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
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
