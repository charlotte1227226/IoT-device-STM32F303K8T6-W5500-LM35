/**
******************************************************************************
* @file    w5500_socket.h
* @author  NTUT Chung Po An
* @brief   Header file of W5500 socket driver  
******************************************************************************
*/
#ifndef __W5500_SOCKET_H
#define __W5500_SOCKET_H

#include "w5500.h"

/*TCP/UDP socket state*/
typedef enum {
    W5500_PROTOCOL_TCP = 0x01,
    W5500_PROTOCOL_UDP = 0x02,
    W5500_PROTOCOL_MACRAW = 0x04
} W5500_ProtocolTypeDef;
/*-------------------- */

/*Socket state*/
typedef enum {
    W5500_CR_OPEN       = 0x01,
    W5500_CR_LISTEN     = 0x02,
    W5500_CR_CONNECT    = 0x04,
    W5500_CR_DISCON     = 0x08,
    W5500_CR_CLOSE      = 0x10,
    W5500_CR_SEND       = 0x20,
    W5500_CR_SEND_MAC   = 0x21,
    W5500_CR_SEND_KEEP  = 0x22,
    W5500_CR_RECV       = 0x40
} W5500_SocketCommandTypeDef;

typedef enum {
    SOCK_CLOSED         = 0x00, // Socket 關閉
    SOCK_INIT           = 0x13, // TCP 模式已 OPEN，等待 CONNECT/LISTEN
    SOCK_LISTEN         = 0x14, // TCP Server 等待連線
    SOCK_ESTABLISHED    = 0x17, // TCP 已建立連線
    SOCK_CLOSE_WAIT     = 0x1C, // TCP 對方已斷線，等待我方關閉

    SOCK_UDP            = 0x22, // UDP 模式
    SOCK_MACRAW         = 0x42, // MACRAW 模式（僅限 Socket 0）
    SOCK_SYNSENT        = 0x15, // Client 已送出 SYN，等待 SYN+ACK
    SOCK_SYNRECV        = 0x16, // Server 收到 SYN，等待 ACK

    SOCK_FIN_WAIT       = 0x18, // 等待斷線完成 (Active Close)
    SOCK_CLOSING        = 0x1A, // 正在關閉 (雙方 FIN 已互送)
    SOCK_TIME_WAIT      = 0x1B, // 等待最後 ACK
    SOCK_LAST_ACK       = 0x1D  // 被動關閉，等待對方 ACK
} W5500_SocketStatusTypeDef;
/*------------*/

typedef struct
{
    W5500_DevTypeDef *dev; // pointer to W5500 device structure
    uint8_t socket_id; // socket number
    W5500_ProtocolTypeDef protocol; // protocol type
    uint16_t local_port; // local port, Sn_PORT
    uint8_t remote_ip[4]; // remote IP address, Sn_DIPR[0~3]
    uint16_t remote_port; // remote port, Sn_DPORT
    uint8_t *tx_buffer; // transmit buffer
    uint8_t *rx_buffer; // receive buffer
    uint16_t tx_size; // transmit buffer size
    uint16_t rx_size; // receive buffer size
    uint16_t tx_write_ptr; // transmit write pointer, Sn_TX_WR
    uint16_t tx_read_ptr; // transmit read pointer, 軟體內部記錄
    W5500_SocketStatusTypeDef state; // socket state
} W5500_SocketTypeDef;

#define W5500_MAX_SOCKETS 8

#define W5500_Socket0 0
#define W5500_Socket1 1
#define W5500_Socket2 2
#define W5500_Socket3 3
#define W5500_Socket4 4
#define W5500_Socket5 5
#define W5500_Socket6 6
#define W5500_Socket7 7

W5500_StatusTypeDef W5500_Socket_Init(uint8_t spi_id,uint8_t socket_num, W5500_SocketTypeDef *socket);
W5500_StatusTypeDef W5500_Socket_Open(uint8_t spi_id,uint8_t socket_num, W5500_SocketTypeDef *sock);
W5500_StatusTypeDef W5500_Socket_Listen(uint8_t spi_id,uint8_t socket_num, W5500_SocketTypeDef *sock);
W5500_StatusTypeDef W5500_Socket_Connect(uint8_t spi_id,uint8_t socket_num, W5500_SocketTypeDef *socket);
W5500_StatusTypeDef W5500_Socket_Send(uint8_t spi_id,uint8_t socket_num, W5500_SocketTypeDef *socket, uint8_t *data, uint16_t size);
W5500_StatusTypeDef W5500_Socket_Receive(uint8_t spi_id,uint8_t socket_num, W5500_SocketTypeDef *socket, uint8_t *data, uint16_t size);
W5500_StatusTypeDef W5500_Socket_Close(uint8_t spi_id,uint8_t socket_num, W5500_SocketTypeDef *socket);
W5500_StatusTypeDef W5500_Socket_Get_Status(uint8_t spi_id,uint8_t socket_num, W5500_SocketTypeDef *socket, uint8_t *status);

#endif