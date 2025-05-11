/**
******************************************************************************
* @file    w5500.h
* @author  NTUT Chung Po An
* @brief   Header file of W5500 spi driver  
******************************************************************************
*/
#ifndef __W5500_SPI_H
#define __W5500_SPI_H
#include "stm32f3xx_hal.h"
#include "spi.h"
#include "gpio.h"
#include <stdint.h>

typedef enum
{
    SPI_OK = 0,
    SPI_ERROR = 1,
    SPI_TIMEOUT = 2,
    SPI_BUSY = 3,
} SPI_StatusTypeDef;

/*Definition of spi id*/
#define SPI1_ID 1
#define SPI2_ID 2
#define SPI3_ID 3
/*end of definition*/

#define SPI_TIMEOUT_VALUE 1000 //ms

SPI_StatusTypeDef SPI_Init(SPI_HandleTypeDef *hspi);
SPI_StatusTypeDef SPI_Write(SPI_HandleTypeDef *hspi, uint8_t *reg_addr, uint8_t *pData, uint16_t Size, uint32_t addr_size);
SPI_StatusTypeDef SPI_Read(SPI_HandleTypeDef *hspi, uint8_t *reg_addr, uint8_t *pData, uint16_t Size, uint32_t addr_size);
void SPI_CS_ENABLE(SPI_HandleTypeDef *hspi);
void SPI_CS_DISABLE(SPI_HandleTypeDef *hspi);
void SPI_Reset(SPI_HandleTypeDef *hspi);
void SPI_Delay(uint32_t delay);

#endif