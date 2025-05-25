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
    uint8_t socket_command = W5500_CR_CLOSE;
    socket -> socket_id = socket_num;
    socket -> protocol = W5500_PROTOCOL_TCP;
    uint8_t socket_bsb = W5500_BSB_SOCKET_REG(socket_num);
    socket -> local_port = 5000;
    socket -> state = SOCK_INIT;
    if(W5500_Write_Byte(spi_id, socket_bsb, W5500_Sn_CR, &socket_command) != W5500_OK){
        return W5500_ERROR;
    }
    if(W5500_Write_Byte(spi_id, socket_bsb, W5500_Sn_MR, (uint8_t *)&(socket->protocol)) != W5500_OK){
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
    /*------------*/
    socket_command = W5500_CR_OPEN;
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
    return W5500_OK;
}