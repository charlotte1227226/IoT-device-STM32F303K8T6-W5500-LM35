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
#define W5500_INTLEVEL0        0x0014 
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
#define W5500_Sn_MR(n)         (0x0000 + (n) * 0x0400) // socket n Mode Register
#endif