/**
******************************************************************************
* @file    w5500_socket.c
* @author  NTUT Chung Po An
* @brief   W5500 socket driver  
******************************************************************************
*/
#include "w5500_socket.h"

W5500_StatusTypeDef W5500_Socket_Init(uint8_t spi_id,uint8_t socket_num, W5500_SocketTypeDef *socket){
    if (socket_num >= W5500_MAX_SOCKETS) {
        return W5500_ERROR;
    }
    socket -> socket_id = socket_num;
    socket -> protocol = W5500_PROTOCOL_TCP;
    uint8_t socket_bsb = W5500_BSB_SOCKET_REG(socket_num);
    socket -> local_port = 5000;
    socket -> tx_size = 2;
    socket -> rx_size = 2;
    socket -> ttl = 128;
    if(W5500_Socket_Close(spi_id, socket_num, socket) != W5500_OK){
        return W5500_ERROR;
    }
    if(W5500_Write_Byte(spi_id, socket_bsb, W5500_Sn_MR, (uint8_t *)&(socket->protocol)) != W5500_OK){
        return W5500_ERROR;
    }
    if(W5500_Write_Byte(spi_id, socket_bsb, W5500_Sn_TXBUF_SIZE, &(socket->tx_size)) != W5500_OK){
        return W5500_ERROR;
    }
    if(W5500_Write_Byte(spi_id, socket_bsb, W5500_Sn_RXBUF_SIZE, &(socket->rx_size)) != W5500_OK){
        return W5500_ERROR;
    }
    if(W5500_Write_Byte(spi_id, socket_bsb, W5500_Sn_TTL, &(socket->ttl)) != W5500_OK){
        return W5500_ERROR;
    }
    uint8_t port[2];
    port[0] = ((socket -> local_port) >> 8) & 0xFF;
    port[1] = (socket -> local_port) & 0xFF;
    if(W5500_Write_Byte(spi_id, socket_bsb, W5500_Sn_PORT0, &port[0]) != W5500_OK){
        return W5500_ERROR;
    }
    if(W5500_Write_Byte(spi_id, socket_bsb, W5500_Sn_PORT1, &port[1]) != W5500_OK){
        return W5500_ERROR;
    }
    if(W5500_Socket_Open(spi_id, socket_num, socket) != W5500_OK){
        return W5500_ERROR;
    }
    return W5500_OK;
}

W5500_StatusTypeDef W5500_Socket_Open(uint8_t spi_id,uint8_t socket_num, W5500_SocketTypeDef *socket){
    uint8_t socket_command = W5500_CR_OPEN;
    uint8_t socket_bsb = W5500_BSB_SOCKET_REG(socket_num);
    uint8_t check_cr;
    uint32_t timeout = 1000;
    do {
        if (W5500_Read_Byte(spi_id, socket_bsb, W5500_Sn_CR, &check_cr) != W5500_OK)
            return W5500_ERROR;
    } while (check_cr != 0x00 && --timeout > 0);

    if(W5500_Write_Byte(spi_id, socket_bsb, W5500_Sn_CR, &socket_command) != W5500_OK){
        return W5500_ERROR;
    }
    uint8_t sr;
    if(W5500_Read_Byte(spi_id, socket_bsb, W5500_Sn_SR, &sr) != W5500_OK){
        return W5500_ERROR;
    }
    if (sr != SOCK_INIT) {
        return W5500_ERROR;
    }
    socket -> state = SOCK_INIT;
    return W5500_OK;
}

W5500_StatusTypeDef W5500_Socket_Close(uint8_t spi_id,uint8_t socket_num, W5500_SocketTypeDef *socket){
    uint8_t socket_command = W5500_CR_CLOSE;
    uint8_t socket_bsb = W5500_BSB_SOCKET_REG(socket_num);
    uint8_t check_cr;
    uint32_t timeout = 1000;
    do {
        if (W5500_Read_Byte(spi_id, socket_bsb, W5500_Sn_CR, &check_cr) != W5500_OK)
            return W5500_ERROR;
    } while (check_cr != 0x00 && --timeout > 0);

    if(W5500_Write_Byte(spi_id, socket_bsb, W5500_Sn_CR, &socket_command) != W5500_OK){
        return W5500_ERROR;
    }
    uint8_t sr;
    if(W5500_Read_Byte(spi_id, socket_bsb, W5500_Sn_SR, &sr) != W5500_OK){
        return W5500_ERROR;
    }
    if (sr != SOCK_CLOSED) {
        return W5500_ERROR;
    }
    socket -> state = SOCK_CLOSED;
    return W5500_OK;
}
