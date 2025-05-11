/**
******************************************************************************
* @file    w5500.h
* @author  NTUT Chung Po An
* @brief   Header file of W5500 driver  
******************************************************************************
*/
#ifndef __W5500_H
#define __W5500_H
/* This is for STM32 HAL library */
#include "stm32f3xx_hal.h"
/* ----------------------------- */
#include "w5500_spi.h"

typedef enum {
    W5500_OK = 0,
    W5500_ERROR = 1,
    W5500_WRITE_ERROR = 2,
    W5500_READ_ERROR = 3,
    W5500_TIMEOUT = 4,
    W5500_DATA_READY = 5,
    W5500_DATA_NOT_READY = 6,
    W5500_BUSY = 7,
} W5500_StatusTypeDef;

typedef struct {
    uint8_t MAC[6]; // MAC address
    uint8_t IP[4]; // IP address
    uint8_t SUBNET[4]; // Subnet mask
    uint8_t GATEWAY[4]; // Gateway address
    uint16_t RetryTime; // Retry time
    uint8_t RetryCount; // Retry count
    uint8_t PHYCFGR; // PHY configuration
    uint8_t Version; // Version
} W5500_DevTypeDef;


/* Operation Mode */
#define W5500_OM_VDM   0x00
#define W5500_OM_FDM1  0x01
#define W5500_OM_FDM2  0x02
/* -------------- */


/* Read or Write */
#define W5500_READ     0x00
#define W5500_WRITE    0x01
/* ------------- */


/* Socket Number Block (BSB)  */
#define W5500_BSB_COMMON        0x00
#define W5500_BSB_SOCKET_REG(n)     ((uint8_t)(0x01 + ((n) << 2)))
#define W5500_BSB_SOCKET_TX(n)      ((uint8_t)(0x02 + ((n) << 2)))
#define W5500_BSB_SOCKET_RX(n)      ((uint8_t)(0x03 + ((n) << 2)))
/* -------------------------  */


/* Control Byte */
static inline uint8_t W5500_CONTROL_BYTE(uint8_t bsb, uint8_t rw, uint8_t om) {
    return (uint8_t)((bsb << 3) | (rw << 2) | (om));
}
// static 不會被外部檔案看到或重複定義。
// inline 函式：適合非常短小、頻繁呼叫的工具函式。放在.h檔案
/* ------------ */


/* W5500 register definition ------------------------------------------------*/
/* Offset Address for Common Register ---------------------------------------*/
#define W5500_MR               0x0000 // Mode Register
#define W5500_GAR0             0x0001 // Gateway Address Register
#define W5500_GAR1             0x0002
#define W5500_GAR2             0x0003
#define W5500_GAR3             0x0004
#define W5500_SUBR0            0x0005 // Subnet Mask Register
#define W5500_SUBR1            0x0006
#define W5500_SUBR2            0x0007
#define W5500_SUBR3            0x0008
#define W5500_SHAR0            0x0009 // Source Hardware Address Register
#define W5500_SHAR1            0x000A 
#define W5500_SHAR2            0x000B
#define W5500_SHAR3            0x000C
#define W5500_SHAR4            0x000D
#define W5500_SHAR5            0x000E
#define W5500_SIPR0            0x000F // Source IP Address Register
#define W5500_SIPR1            0x0010
#define W5500_SIPR2            0x0011
#define W5500_SIPR3            0x0012
#define W5500_INTLEVEL0        0x0013 // Interrupt Low Level Timer 
#define W5500_INTLEVEL1        0x0014 
#define W5500_IR               0x0015 // Interrupt Register
#define W5500_IMR              0x0016 // Interrupt Mask Register
#define W5500_SIR              0x0017 // Socket Interrupt Register
#define W5500_SIMR             0x0018 // Socket Interrupt Mask Register
#define W5500_RTR0             0x0019 // Retry Time Register
#define W5500_RTR1             0x001A
#define W5500_RCR              0x001B // Retry Count Register
#define W5500_PTIMER           0x001C // PPP LCP Request Timer Register
#define W5500_PMAGIC           0x001D // PPP LCP Magic Number Register
#define W5500_PHAR0            0x001E // PPP Destination MAC Register
#define W5500_PHAR1            0x001F
#define W5500_PHAR2            0x0020
#define W5500_PHAR3            0x0021
#define W5500_PHAR4            0x0022
#define W5500_PHAR5            0x0023
#define W5500_PSID0            0x0024 // PPP Session ID Register
#define W5500_PSID1            0x0025
#define W5500_PMRU0            0x0026 // PPP MRU Register
#define W5500_PMRU1            0x0027
#define W5500_UIPR0            0x0028 // Unreachable IP Address Register
#define W5500_UIPR1            0x0029
#define W5500_UIPR2            0x002A
#define W5500_UIPR3            0x002B
#define W5500_UPORT0           0x002C // Unreachable Port Register
#define W5500_UPORT1           0x002D
#define W5500_PHYCFGR          0x002E // PHY Configuration Register
#define W5500_VERSIONR         0x0039 // Version Register
/* Offset Address  in Socket n Register Block (0≤n≤7) -----------------------*/
#define W5500_Sn_MR            0x0000 // socket n Mode Register
#define W5500_Sn_CR            0x0001 // socket n Command Register
#define W5500_Sn_IR            0x0002 // socket n Interrupt Register
#define W5500_Sn_SR            0x0003 // socket n Status Register
#define W5500_Sn_PORT0         0x0004 // socket n Source Port Register
#define W5500_Sn_PORT1         0x0005
#define W5500_Sn_DHAR0         0x0006 // socket n Destination Hardware Address Register
#define W5500_Sn_DHAR1         0x0007
#define W5500_Sn_DHAR2         0x0008
#define W5500_Sn_DHAR3         0x0009
#define W5500_Sn_DHAR4         0x000A
#define W5500_Sn_DHAR5         0x000B
#define W5500_Sn_DIPR0         0x000C // socket n Destination IP Address Register
#define W5500_Sn_DIPR1         0x000D
#define W5500_Sn_DIPR2         0x000E
#define W5500_Sn_DIPR3         0x000F
#define W5500_Sn_DPORT0        0x0010 // socket n Destination Port Register
#define W5500_Sn_DPORT1        0x0011
#define W5500_Sn_MSSR0         0x0012 // socket n Maximum Segment Size Register
#define W5500_Sn_MSSR1         0x0013
#define W5500_Sn_TOS           0x0015 // socket n Type of Service Register
#define W5500_Sn_TTL           0x0016 // socket n Time to Live Register
#define W5500_Sn_RXBUF_SIZE    0x001E // socket n RX Buffer Size Register
#define W5500_Sn_TXBUF_SIZE    0x001F // socket n TX Buffer Size Register
#define W5500_Sn_TX_FSR0       0x0020 // socket n TX Free Size Register
#define W5500_Sn_TX_FSR1       0x0021
#define W5500_Sn_TX_RD0        0x0022 // socket n TX Read Pointer Register
#define W5500_Sn_TX_RD1        0x0023
#define W5500_Sn_TX_WR0        0x0024 // socket n TX Write Pointer Register
#define W5500_Sn_TX_WR1        0x0025
#define W5500_Sn_RX_RSR0       0x0026 // socket n RX Received Size Register
#define W5500_Sn_RX_RSR1       0x0027
#define W5500_Sn_RX_RD0        0x0028 // socket n RX Read Pointer Register
#define W5500_Sn_RX_RD1        0x0029
#define W5500_Sn_RX_WR0        0x002A // socket n RX Write Pointer Register
#define W5500_Sn_RX_WR1        0x002B
#define W5500_Sn_IMR           0x002C // socket n Interrupt Mask Register
#define W5500_Sn_FRAG0         0x002D // socket n Fragment Register
#define W5500_Sn_FRAG1         0x002E
#define W5500_Sn_KPALVTR       0x002F // socket n Keep Alive Timer Register

W5500_StatusTypeDef W5500_Init(SPI_HandleTypeDef *hspi, W5500_DevTypeDef *dev);
W5500_StatusTypeDef W5500_Write_Byte(SPI_HandleTypeDef *hspi, uint16_t reg_addr, uint8_t *pData, uint16_t Size);
W5500_StatusTypeDef W5500_Write_Bytes(SPI_HandleTypeDef *hspi, uint16_t reg_addr, uint8_t *pData, uint16_t Size);
W5500_StatusTypeDef W5500_Read_Byte(SPI_HandleTypeDef *hspi, uint16_t reg_addr, uint8_t *pData, uint16_t Size);
W5500_StatusTypeDef W5500_Read_Bytes(SPI_HandleTypeDef *hspi, uint16_t reg_addr, uint8_t *pData, uint16_t Size);
#endif