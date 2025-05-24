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
    SOCKET_STATE_CLOSED = 0x00,
    SOCKET_STATE_INIT = 0x13,
    SOCKET_STATE_LISTEN = 0x14,
    SOCKET_STATE_ESTABLISHED = 0x17,
    SOCKET_STATE_UDP = 0x22
} W5500_SocketStateTypeDef;
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
    W5500_SocketStateTypeDef state; // socket state
} W5500_SocketTypeDef;

#define W5500_MAX_SOCKETS 8

W5500_StatusTypeDef W5500_Socket_Init(W5500_SocketTypeDef *socket);
W5500_StatusTypeDef W5500_Socket_Open(   W5500_SocketTypeDef *sock);
W5500_StatusTypeDef W5500_Socket_Listen( W5500_SocketTypeDef *sock);
W5500_StatusTypeDef W5500_Socket_Connect(W5500_SocketTypeDef *socket);
W5500_StatusTypeDef W5500_Socket_Send(W5500_SocketTypeDef *socket, uint8_t *data, uint16_t size);
W5500_StatusTypeDef W5500_Socket_Receive(W5500_SocketTypeDef *socket, uint8_t *data, uint16_t size);
W5500_StatusTypeDef W5500_Socket_Close(W5500_SocketTypeDef *socket);
W5500_StatusTypeDef W5500_Socket_Get_Status(W5500_SocketTypeDef *socket, uint8_t *status);

#endif