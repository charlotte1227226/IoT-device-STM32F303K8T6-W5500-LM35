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

typedef struct
{
    W5500_DevTypeDef *dev; // pointer to W5500 device structure
    uint8_t socket_id; // socket number
    uint8_t protocol; // protocol type
    uint16_t local_port; // local port
    uint8_t remote_ip; // remote IP address
    uint16_t remote_port; // remote port
    uint8_t *tx_buffer; // transmit buffer
    uint8_t *rx_buffer; // receive buffer
    uint16_t tx_size; // transmit buffer size
    uint16_t rx_size; // receive buffer size
    uint16_t tx_write_ptr; // transmit write pointer
    uint16_t tx_read_ptr; // transmit read pointer
    uint8_t state; // socket state
} W5500_SocketTypeDef;

W5500_StatusTypeDef W5500_Socket_Init(W5500_SocketTypeDef *socket);
W5500_StatusTypeDef W5500_Socket_Open(   W5500_SocketTypeDef *sock);
W5500_StatusTypeDef W5500_Socket_Listen( W5500_SocketTypeDef *sock);
W5500_StatusTypeDef W5500_Socket_Connect(W5500_SocketTypeDef *socket);
W5500_StatusTypeDef W5500_Socket_Send(W5500_SocketTypeDef *socket, uint8_t *data, uint16_t size);
W5500_StatusTypeDef W5500_Socket_Receive(W5500_SocketTypeDef *socket, uint8_t *data, uint16_t size);
W5500_StatusTypeDef W5500_Socket_Close(W5500_SocketTypeDef *socket);
W5500_StatusTypeDef W5500_Socket_Get_Status(W5500_SocketTypeDef *socket, uint8_t *status);

#endif