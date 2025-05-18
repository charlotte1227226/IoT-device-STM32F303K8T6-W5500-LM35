/**
******************************************************************************
* @file    w5500.c
* @author  NTUT Chung Po An
* @brief   W5500 driver  
******************************************************************************
*/
#include "w5500.h"

W5500_StatusTypeDef W5500_Init(uint8_t spi_id, W5500_DevTypeDef *dev){
    if(SPI_Init(spi_id) != SPI_OK){
        return W5500_ERROR;
    }
    return W5500_OK;
}

W5500_StatusTypeDef W5500_Write_Byte(uint8_t spi_id, uint16_t reg_addr, uint8_t *pData, uint16_t Size){
    uint8_t addr[2];
    addr[0] = (reg_addr >> 8) & 0xFF;
    addr[1] = reg_addr & 0xFF;
    SPI_CS_ENABLE(spi_id);
    if(SPI_Write(spi_id, addr, pData, Size, 2) != SPI_OK){
        SPI_CS_DISABLE(spi_id);
        return W5500_WRITE_ERROR;
    }
}