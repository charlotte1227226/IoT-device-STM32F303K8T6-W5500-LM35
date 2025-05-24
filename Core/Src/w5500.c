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
    // 1. 清除 W5500 軟體重置（MR 寄存器 bit7）
    uint8_t mr_reset_clear = 0x00;
    if(W5500_Write_Byte(SPI1_ID, W5500_BSB_COMMON, W5500_MR, &mr_reset_clear) != W5500_OK) {
        printf("清除 W5500 軟體重置失敗\n");
    } 
    else {
        printf("清除 W5500 軟體重置成功\n");
    }
    SPI_Delay(2);  // 等待重置完成
    dev -> MAC[0] = 0x00;
    dev -> MAC[1] = 0x08;
    dev -> MAC[2] = 0xDC;
    dev -> MAC[3] = 0x12;
    dev -> MAC[4] = 0x34;
    dev -> MAC[5] = 0x56;
    dev -> IP[0] = 192;
    dev -> IP[1] = 168;
    dev -> IP[2] = 0;
    dev -> IP[3] = 20;
    dev -> SUBNET[0] = 255;
    dev -> SUBNET[1] = 255;
    dev -> SUBNET[2] = 255;
    dev -> SUBNET[3] = 0;
    dev -> GATEWAY[0] = 192;
    dev -> GATEWAY[1] = 168;
    dev -> GATEWAY[2] = 0;
    dev -> GATEWAY[3] = 1;
    dev -> RetryTime = 2000;
    dev -> RetryCount = 8;
    dev->PHYCFGR = 0xF8;  // bit7=RST=0, bit6=OPMD=1 (auto-negotiation), bit5~3=OPMDC=111
    dev -> Version = 0x00;
    
    W5500_Write_Bytes(spi_id, W5500_BSB_COMMON, W5500_SHAR0, dev->MAC, 6); 
    W5500_Write_Bytes(spi_id, W5500_BSB_COMMON, W5500_GAR0, dev->GATEWAY, 4); 
    W5500_Write_Bytes(spi_id, W5500_BSB_COMMON, W5500_SUBR0, dev->SUBNET, 4); 
    W5500_Write_Bytes(spi_id, W5500_BSB_COMMON, W5500_SIPR0, dev->IP, 4); 
    uint8_t retry_time[2];
    retry_time[0] = (dev->RetryTime >> 8) & 0xFF;
    retry_time[1] = dev->RetryTime & 0xFF;
    W5500_Write_Bytes(spi_id, W5500_BSB_COMMON, W5500_RTR0, retry_time, 2);
    W5500_Write_Byte(spi_id, W5500_BSB_COMMON, W5500_RCR, &dev->RetryCount);
    W5500_Write_Byte(spi_id, W5500_BSB_COMMON, W5500_PHYCFGR, &dev->PHYCFGR);
    uint8_t version;
    if(W5500_Read_Byte(spi_id, W5500_BSB_COMMON, W5500_VERSIONR, &version) != W5500_OK) {
        printf("讀取 W5500 版本失敗\n");
        return W5500_ERROR;
    } else {
        dev->Version = version;
        printf("W5500 版本: %02X\n", dev->Version);
    }

    uint8_t zero = 0x00;
    W5500_Write_Byte(spi_id, W5500_BSB_COMMON, W5500_IMR, &zero);
    W5500_Write_Byte(spi_id, W5500_BSB_COMMON, W5500_SIMR, &zero);
    uint8_t clear_ir = 0xF0;  // Bit 7~4：CONFLICT, UNREACH, PPPoE, MP
    W5500_Write_Byte(spi_id, W5500_BSB_COMMON, W5500_IR, &clear_ir);
    W5500_Write_Byte(spi_id, W5500_BSB_COMMON, W5500_SIR, &zero);
    return W5500_OK;
}

W5500_StatusTypeDef W5500_Write_Byte(uint8_t spi_id, uint8_t bsb, uint16_t reg_addr, uint8_t *pData){
    uint8_t tx_data[4];
    tx_data[0] = (reg_addr >> 8) & 0xFF;
    tx_data[1] = reg_addr & 0xFF;
    tx_data[2] = W5500_CONTROL_BYTE(bsb, W5500_WRITE, W5500_OM_VDM);
    tx_data[3] = *pData;
    SPI_CS_ENABLE(spi_id);
    if(SPI_Write(spi_id, 0, tx_data, 1, 3) != SPI_OK){
        SPI_CS_DISABLE(spi_id);
        return W5500_WRITE_ERROR;
    }
    SPI_CS_DISABLE(spi_id);
    return W5500_OK;
}

W5500_StatusTypeDef W5500_Write_Bytes(uint8_t spi_id, uint8_t bsb, uint16_t reg_addr, uint8_t *pData, uint16_t Size){
    uint8_t tx_data[Size + 3];
    tx_data[0] = (reg_addr >> 8) & 0xFF;
    tx_data[1] = reg_addr & 0xFF;
    tx_data[2] = W5500_CONTROL_BYTE(bsb, W5500_WRITE, W5500_OM_VDM);
    for(int i = 0; i < Size; i++){
        tx_data[i + 3] = pData[i];
    }
    SPI_CS_ENABLE(spi_id);
    if(SPI_Write(spi_id, 0, tx_data, Size, 3) != SPI_OK){
        SPI_CS_DISABLE(spi_id);
        return W5500_WRITE_ERROR;
    }
    SPI_CS_DISABLE(spi_id);
    return W5500_OK;
}

W5500_StatusTypeDef W5500_Read_Byte(uint8_t spi_id, uint8_t bsb, uint16_t reg_addr, uint8_t *pData){
    uint8_t tx_data[3];
    tx_data[0] = (reg_addr >> 8) & 0xFF;
    tx_data[1] = reg_addr & 0xFF;
    tx_data[2] = W5500_CONTROL_BYTE(bsb, W5500_READ, W5500_OM_VDM);
    SPI_CS_ENABLE(spi_id);
    if(SPI_Read(spi_id, tx_data, pData, 1, 3) != SPI_OK){
        SPI_CS_DISABLE(spi_id);
        return W5500_READ_ERROR;
    }
    SPI_CS_DISABLE(spi_id);
    return W5500_OK;
}

W5500_StatusTypeDef W5500_Read_Bytes(uint8_t spi_id, uint8_t bsb, uint16_t reg_addr, uint8_t *pData, uint16_t Size){
    uint8_t tx_data[3];
    tx_data[0] = (reg_addr >> 8) & 0xFF;
    tx_data[1] = reg_addr & 0xFF;
    tx_data[2] = W5500_CONTROL_BYTE(bsb, W5500_READ, W5500_OM_VDM);
    SPI_CS_ENABLE(spi_id);
    if(SPI_Read(spi_id, tx_data, pData, Size, 3) != SPI_OK){
        SPI_CS_DISABLE(spi_id);
        return W5500_READ_ERROR;
    }
    SPI_CS_DISABLE(spi_id);
    return W5500_OK;
}

W5500_StatusTypeDef W5500_Read_Init(uint8_t spi_id, W5500_DevTypeDef *dev){
    if(W5500_Read_Bytes(spi_id, W5500_BSB_COMMON, W5500_SHAR0, dev->MAC, 6) != W5500_OK) {
        printf("讀取 MAC 地址失敗\n");
        return W5500_READ_ERROR;
    }
    if(W5500_Read_Bytes(spi_id, W5500_BSB_COMMON, W5500_SIPR0, dev->IP, 4) != W5500_OK) {
        printf("讀取 IP 地址失敗\n");
        return W5500_READ_ERROR;
    }
    if(W5500_Read_Bytes(spi_id, W5500_BSB_COMMON, W5500_SUBR0, dev->SUBNET, 4) != W5500_OK) {
        printf("讀取子網掩碼失敗\n");
        return W5500_READ_ERROR;
    }
    if(W5500_Read_Bytes(spi_id, W5500_BSB_COMMON, W5500_GAR0, dev->GATEWAY, 4) != W5500_OK) {
        printf("讀取閘道地址失敗\n");
        return W5500_READ_ERROR;
    }
    uint8_t retry_time[2];
    if(W5500_Read_Bytes(spi_id, W5500_BSB_COMMON, W5500_RTR0, retry_time, 2) != W5500_OK) {
        printf("讀取重試時間失敗\n");
        return W5500_READ_ERROR;
    }
    dev->RetryTime = (retry_time[0] << 8) | retry_time[1];
    if(W5500_Read_Byte(spi_id, W5500_BSB_COMMON, W5500_RCR, &dev->RetryCount) != W5500_OK) {
        printf("讀取重試計數失敗\n");
        return W5500_READ_ERROR;
    }
    if(W5500_Read_Byte(spi_id, W5500_BSB_COMMON, W5500_PHYCFGR, &dev->PHYCFGR) != W5500_OK) {
        printf("讀取 PHY 配置失敗\n");
        return W5500_READ_ERROR;
    }
    if(W5500_Read_Byte(spi_id, W5500_BSB_COMMON, W5500_VERSIONR, &dev->Version) != W5500_OK) {
        printf("讀取版本失敗\n");
        return W5500_READ_ERROR;
    }
    printf("W5500 版本: %02X\n", dev->Version);
    return W5500_OK;
}
