/**
******************************************************************************
* @file    w5500.h
* @author  NTUT Chung Po An
* @brief   W5500 spi driver  
******************************************************************************
*/
#include "w5500_spi.h"
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;

SPI_StatusTypeDef SPI_Init(uint8_t spi_id){
    switch (spi_id)
    {
    case SPI1_ID:
        MX_SPI1_Init();
        break;
    // case SPI2_ID:
    //     MX_SPI2_Init();
    //     break;
    default:
        return SPI_ERROR;
    }
    return SPI_OK;
}

SPI_StatusTypeDef SPI_Write(uint8_t spi_id, uint8_t *reg_addr, uint8_t *pData, uint16_t Size, uint32_t addr_size){
    switch (spi_id)
    {
    case SPI1_ID:
        if(HAL_SPI_Transmit(&hspi1, pData, addr_size + Size, SPI_TIMEOUT_VALUE) != HAL_OK){
            return SPI_ERROR;
        }
        break;
    // case SPI2_ID:
    //     if(HAL_SPI_Transmit(&hspi2, pData, addr_size + Size, SPI_TIMEOUT_VALUE) != HAL_OK){
    //         return SPI_ERROR;
    //     }
    //     break;
    default:
        return SPI_ERROR;
    }
    return SPI_OK;
}

SPI_StatusTypeDef SPI_Read(uint8_t spi_id, uint8_t *reg_addr, uint8_t *pData, uint16_t Size, uint32_t addr_size){
    switch (spi_id)
    {
    case SPI1_ID:
        if(HAL_SPI_Transmit(&hspi1, reg_addr, addr_size, SPI_TIMEOUT_VALUE) != HAL_OK){
            return SPI_ERROR;
        }
        if(HAL_SPI_Receive(&hspi1, pData, Size, SPI_TIMEOUT_VALUE) != HAL_OK){
            return SPI_ERROR;
        }
        break;
    // case SPI2_ID:
    //     if(HAL_SPI_Transmit(&hspi2, reg_addr, addr_size, SPI_TIMEOUT_VALUE) != HAL_OK){
    //         return SPI_ERROR;
    //     }
    //     if(HAL_SPI_Receive(&hspi2, pData, Size, SPI_TIMEOUT_VALUE) != HAL_OK){
    //         return SPI_ERROR;
    //     }
    //     break;
    default:
        return SPI_ERROR;
    }
    return SPI_OK;
}

void SPI_CS_ENABLE(uint8_t spi_id){
    switch (spi_id)
    {
    case SPI1_ID:
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
        break;
    // case SPI2_ID:
    //     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    //     break;
    default:
        break;
    }
}

void SPI_CS_DISABLE(uint8_t spi_id){
    switch (spi_id)
    {
    case SPI1_ID:
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
        break;
    // case SPI2_ID:
    //     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    //     break;
    default:
        break;
    }
}

void SPI_Reset(uint8_t spi_id){
    switch (spi_id)
    {
    case SPI1_ID:
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
        HAL_Delay(1);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
        break;
    // case SPI2_ID:
    //     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    //     HAL_Delay(1);
    //     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    //     break;
    default:
        break;
    }
}

void SPI_Delay(uint32_t delay){
    HAL_Delay(delay);
}