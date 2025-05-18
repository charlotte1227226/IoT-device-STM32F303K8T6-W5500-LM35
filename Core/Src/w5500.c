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
    dev -> MAC[0] = 0x00;
    dev -> MAC[1] = 0x08;
    dev -> MAC[2] = 0xDC;
    dev -> MAC[3] = 0x12;
    dev -> MAC[4] = 0x34;
    dev -> MAC[5] = 0x56;
    dev -> IP[0] = 192;
    dev -> IP[1] = 168;
    dev -> IP[2] = 0;
    dev -> IP[3] = 100;
    dev -> SUBNET[0] = 255;
    dev -> SUBNET[1] = 255;
    dev -> SUBNET[2] = 255;
    dev -> SUBNET[3] = 0;
    dev -> GATEWAY[0] = 192;
    dev -> GATEWAY[1] = 168;
    dev -> GATEWAY[2] = 0;
    dev -> GATEWAY[3] = 1;
    dev -> RetryTime = 0;
    dev -> RetryCount = 0;
    dev -> PHYCFGR = 0x00;
    dev -> Version = 0x00;
    return W5500_OK;
}

W5500_StatusTypeDef W5500_Write_Byte(uint8_t spi_id, uint16_t reg_addr, uint8_t *pData){
    uint8_t tx_data[4];
    tx_data[0] = (reg_addr >> 8) & 0xFF;
    tx_data[1] = reg_addr & 0xFF;
    tx_data[2] = W5500_CONTROL_BYTE(W5500_BSB_COMMON, W5500_WRITE, W5500_OM_VDM);
    tx_data[3] = *pData;
    SPI_CS_ENABLE(spi_id);
    if(SPI_Write(spi_id, 0, tx_data, 1, 3) != SPI_OK){
        SPI_CS_DISABLE(spi_id);
        return W5500_WRITE_ERROR;
    }
    return W5500_OK;
}

W5500_StatusTypeDef W5500_Write_Bytes(uint8_t spi_id, uint16_t reg_addr, uint8_t *pData, uint16_t Size){
    uint8_t tx_data[Size + 3];
    tx_data[0] = (reg_addr >> 8) & 0xFF;
    tx_data[1] = reg_addr & 0xFF;
    tx_data[2] = W5500_CONTROL_BYTE(W5500_BSB_COMMON, W5500_WRITE, W5500_OM_VDM);
    for(int i = 0; i < Size; i++){
        tx_data[i + 3] = pData[i];
    }
    SPI_CS_ENABLE(spi_id);
    if(SPI_Write(spi_id, 0, tx_data, Size, 3) != SPI_OK){
        SPI_CS_DISABLE(spi_id);
        return W5500_WRITE_ERROR;
    }
    return W5500_OK;
}

W5500_StatusTypeDef W5500_Read_Byte(uint8_t spi_id, uint16_t reg_addr, uint8_t *pData){
    uint8_t tx_data[3];
    tx_data[0] = (reg_addr >> 8) & 0xFF;
    tx_data[1] = reg_addr & 0xFF;
    tx_data[2] = W5500_CONTROL_BYTE(W5500_BSB_COMMON, W5500_READ, W5500_OM_VDM);
    SPI_CS_ENABLE(spi_id);
    if(SPI_Read(spi_id, tx_data, pData, 1, 3) != SPI_OK){
        SPI_CS_DISABLE(spi_id);
        return W5500_READ_ERROR;
    }
    return W5500_OK;
}

W5500_StatusTypeDef W5500_Read_Bytes(uint8_t spi_id, uint16_t reg_addr, uint8_t *pData, uint16_t Size){
    uint8_t tx_data[3];
    tx_data[0] = (reg_addr >> 8) & 0xFF;
    tx_data[1] = reg_addr & 0xFF;
    tx_data[2] = W5500_CONTROL_BYTE(W5500_BSB_COMMON, W5500_READ, W5500_OM_VDM);
    SPI_CS_ENABLE(spi_id);
    if(SPI_Read(spi_id, tx_data, pData, Size, 3) != SPI_OK){
        SPI_CS_DISABLE(spi_id);
        return W5500_READ_ERROR;
    }
    return W5500_OK;
}
