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
    if(W5500_Write_Bytes(spi_id, socket_bsb, W5500_Sn_PORT0, port, 2) != W5500_OK){
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
        if (W5500_Read_Byte(spi_id, socket_bsb, W5500_Sn_CR, &check_cr) != W5500_OK){
            printf("wait\n");
        }
    } while (check_cr != 0x00 && --timeout > 0);
    if (timeout == 0) {
        printf("Timeout waiting for Sn_CR to clear\n");
        return W5500_TIMEOUT;
    }
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
        if (W5500_Read_Byte(spi_id, socket_bsb, W5500_Sn_CR, &check_cr) != W5500_OK){
            printf("wait\n");
        }
    } while (check_cr != 0x00 && --timeout > 0);
    if (timeout == 0) {
        printf("Timeout waiting for Sn_CR to clear\n");
        return W5500_TIMEOUT;
    }
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

//TCP mode
W5500_StatusTypeDef W5500_Socket_Listen(uint8_t spi_id,uint8_t socket_num, W5500_SocketTypeDef *socket){
    uint8_t socket_command = W5500_CR_LISTEN;
    uint8_t socket_bsb = W5500_BSB_SOCKET_REG(socket_num);
    uint8_t check_cr, check_sr;
    uint32_t timeout = 1000;
    if(W5500_Read_Byte(spi_id, socket_bsb, W5500_Sn_SR, &check_sr) != W5500_OK){
        return W5500_ERROR;
    }
    if (check_sr != SOCK_INIT) {
        return W5500_ERROR;
    }
    do {
        if (W5500_Read_Byte(spi_id, socket_bsb, W5500_Sn_CR, &check_cr) != W5500_OK){
            printf("wait\n");
        }
    } while (check_cr != 0x00 && --timeout > 0);
    if (timeout == 0) {
        printf("Timeout waiting for Sn_CR to clear\n");
        return W5500_TIMEOUT;
    }
    if(W5500_Write_Byte(spi_id, socket_bsb, W5500_Sn_CR, &socket_command) != W5500_OK){;
        return W5500_ERROR;
    }
    uint8_t sr;
    if(W5500_Read_Byte(spi_id, socket_bsb, W5500_Sn_SR, &sr) != W5500_OK){
        return W5500_ERROR;
    }
    if (sr == SOCK_CLOSED) {
        printf("LISTEN 失敗：Socket %d 被關閉\n", socket_num);
        socket->state = SOCK_CLOSED;
        return W5500_ERROR;
    }
    if (sr != SOCK_LISTEN) {
        printf("Sn_SR NOT SOCK_LISTEN: 0x%02X\n", sr);
        return W5500_ERROR;
    }
    socket -> state = SOCK_LISTEN;
    printf("Sn_SR before LISTEN: 0x%02X\n", check_sr);
    printf("Sn_SR after LISTEN: 0x%02X\n", sr);
    return W5500_OK;
}

W5500_StatusTypeDef W5500_Socket_Check_Established(uint8_t spi_id,uint8_t socket_num, W5500_SocketTypeDef *socket){
    uint8_t socket_bsb = W5500_BSB_SOCKET_REG(socket_num);
    uint8_t check_sr;
    uint32_t timeout = 1000;
    if(W5500_Read_Byte(spi_id, socket_bsb, W5500_Sn_SR, &check_sr) != W5500_OK){
        printf("Establish 讀取 Sn_SR 失敗\n");
        return W5500_ERROR;
    }
    if (check_sr == SOCK_CLOSED) {
        printf("Socket %d 連線被關閉 (SOCK_CLOSED)\n", socket_num);
        socket->state = SOCK_CLOSED;
        return W5500_ERROR;
    }
    if (check_sr != SOCK_ESTABLISHED) {
        printf("Sn_SR 不是 SOCK_ESTABLISHED\n");
        printf("Sn_SR = 0x%02X\n", check_sr);
        return W5500_ERROR;
    }
    socket -> state = SOCK_ESTABLISHED;
    printf("Socket %d 已建立連線！Sn_SR = 0x%02X\n", socket_num, check_sr);
    return W5500_OK;
}

W5500_StatusTypeDef W5500_Socket_Connect(uint8_t spi_id,uint8_t socket_num, W5500_SocketTypeDef *socket){
    uint8_t socket_bsb = W5500_BSB_SOCKET_REG(socket_num);
    socket -> remote_ip[0] = 192;
    socket -> remote_ip[1] = 168;
    socket -> remote_ip[2] = 0;
    socket -> remote_ip[3] = 35;
    socket -> remote_port = 80;
    if(W5500_Write_Bytes(spi_id, socket_bsb, W5500_Sn_DIPR0, socket->remote_ip, 4) != W5500_OK){;
        return W5500_ERROR;
    }
    uint8_t port[2];
    port[0] = (socket->remote_port >> 8) & 0xFF;
    port[1] = socket->remote_port & 0xFF;
    if(W5500_Write_Bytes(spi_id, socket_bsb, W5500_Sn_DPORT0, port, 2) != W5500_OK){
        return W5500_ERROR;
    }
    uint8_t socket_command = W5500_CR_CONNECT;
    if(W5500_Write_Byte(spi_id, socket_bsb, W5500_Sn_CR, &socket_command) != W5500_OK){
        return W5500_ERROR;
    }
    uint8_t check_cr;
    uint32_t timeout = 1000;
    do {
        if(W5500_Read_Byte(spi_id, socket_bsb, W5500_Sn_CR, &check_cr) != W5500_OK){
            printf("wait\n");
        }
    } while (check_cr != 0x00 && --timeout > 0);
    if(timeout == 0){
        printf("CONNECT timeout: Sn_CR 未變為 0x00\n");
        return W5500_TIMEOUT;
    }
    uint8_t check_sr;
    timeout = 1000;
    do {
        W5500_Read_Byte(spi_id, socket_bsb, W5500_Sn_SR, &check_sr);
    } while (check_sr != SOCK_ESTABLISHED && --timeout > 0);
    if(timeout == 0){
        if(check_sr == SOCK_CLOSED) {
            printf("CONNECT 失敗：連線被拒絕或超時，Sn_SR = 0x%02X\n", check_sr);
        } else {
            printf("CONNECT 逾時，Sn_SR = 0x%02X\n", check_sr);
        }
        return W5500_TIMEOUT;
    }
    socket->state = SOCK_ESTABLISHED;
    return W5500_OK;
}

W5500_StatusTypeDef W5500_Socket_Send(uint8_t spi_id,uint8_t socket_num, W5500_SocketTypeDef *socket, uint8_t *data, uint16_t size){
    if(W5500_Socket_Check_Established(spi_id, socket_num, socket) != W5500_OK){
        return W5500_ERROR;
    }
    uint8_t socket_bsb = W5500_BSB_SOCKET_REG(socket_num);
    uint8_t check_tx_size[2];
    if(W5500_Read_Bytes(spi_id, socket_bsb, W5500_Sn_TX_FSR0, check_tx_size, 2) != W5500_OK){
        return W5500_ERROR;
    }
    uint16_t tx_available_size;
    tx_available_size = (((uint16_t)check_tx_size[0] << 8) | check_tx_size[1]);
    if(tx_available_size < size){
        return W5500_BUSY;
    }
    uint8_t tx_wr_bytes[2];
    if(W5500_Read_Bytes(spi_id, socket_bsb, W5500_Sn_TX_WR0, tx_wr_bytes, 2) != W5500_OK){
        return W5500_ERROR;
    }
    uint16_t tx_wr_ptr;
    tx_wr_ptr = ((uint16_t)tx_wr_bytes[0] << 8) | tx_wr_bytes[1];
    uint16_t addr = tx_wr_ptr & (socket->tx_size * 1024 - 1);
    if(W5500_Write_Bytes(spi_id, W5500_BSB_SOCKET_TX(socket_num), addr, data, size) != W5500_OK){
        return W5500_ERROR;
    }
    tx_wr_ptr += size;
    tx_wr_bytes[0] = (tx_wr_ptr >> 8) & 0xFF;
    tx_wr_bytes[1] = tx_wr_ptr & 0xFF;
    if(W5500_Write_Bytes(spi_id, socket_bsb, W5500_Sn_TX_WR0, tx_wr_bytes, 2) != W5500_OK){
        return W5500_ERROR;
    }
    uint8_t socket_command = W5500_CR_SEND;
    if(W5500_Write_Byte(spi_id, socket_bsb, W5500_Sn_CR, &socket_command) != W5500_OK){
        return W5500_ERROR;
    }
    uint8_t cr;
    uint32_t timeout = 1000;
    do {
        if (W5500_Read_Byte(spi_id, socket_bsb, W5500_Sn_CR, &cr) != W5500_OK){
            return W5500_ERROR;
        }
    } while (cr != 0x00 && --timeout > 0);
    if (timeout == 0) {
        return W5500_TIMEOUT;
    }
    // 檢查 Sn_IR 的 SEND_OK 與 TIMEOUT 狀態
    uint8_t ir;
    if (W5500_Read_Byte(spi_id, socket_bsb, W5500_Sn_IR, &ir) != W5500_OK) {
        return W5500_ERROR;
    }
    if (ir & 0x10) {  // Bit 4: SEND_OK
        uint8_t clear = 0x10;
        if(W5500_Write_Byte(spi_id, socket_bsb, W5500_Sn_IR, &clear) != W5500_OK){
            return W5500_ERROR;
        }
        printf("SEND OK, Sn_IR = 0x%02X\n", ir);
    } else if (ir & 0x08) { // Bit 3: TIMEOUT
        printf("SEND failed: TIMEOUT, Sn_IR = 0x%02X\n", ir);
        return W5500_TIMEOUT;
    }
    return W5500_OK;
}

W5500_StatusTypeDef W5500_Socket_Receive(uint8_t spi_id, uint8_t socket_num, W5500_SocketTypeDef *socket, uint8_t *data, uint16_t size) {
    if (W5500_Socket_Check_Established(spi_id, socket_num, socket) != W5500_OK) {
        return W5500_ERROR;
    }

    uint8_t socket_bsb = W5500_BSB_SOCKET_REG(socket_num);
    uint8_t rx_size_bytes[2];

    // 1. 讀取接收緩衝區中已接收的資料大小 Sn_RX_RSR
    if (W5500_Read_Bytes(spi_id, socket_bsb, W5500_Sn_RX_RSR0, rx_size_bytes, 2) != W5500_OK) {
        return W5500_ERROR;
    }

    uint16_t rx_data_size = ((uint16_t)rx_size_bytes[0] << 8) | rx_size_bytes[1];
    if (rx_data_size == 0) {
        return W5500_DATA_NOT_READY;
    }
    if (rx_data_size < size) {
        size = rx_data_size;  // 若資料量不足，僅讀可讀取的部分
    }

    // 2. 取得當前的接收讀取指標 Sn_RX_RD
    uint8_t rx_rd_ptr_bytes[2];
    if (W5500_Read_Bytes(spi_id, socket_bsb, W5500_Sn_RX_RD0, rx_rd_ptr_bytes, 2) != W5500_OK) {
        return W5500_ERROR;
    }

    uint16_t rx_rd_ptr = ((uint16_t)rx_rd_ptr_bytes[0] << 8) | rx_rd_ptr_bytes[1];
    uint16_t addr = rx_rd_ptr & (socket->rx_size * 1024 - 1);

    // 3. 從對應地址讀出資料
    if (W5500_Read_Bytes(spi_id, W5500_BSB_SOCKET_RX(socket_num), addr, data, size) != W5500_OK) {
        return W5500_ERROR;
    }

    // 4. 更新 Sn_RX_RD 為新的讀取位置
    rx_rd_ptr += size;
    rx_rd_ptr_bytes[0] = (rx_rd_ptr >> 8) & 0xFF;
    rx_rd_ptr_bytes[1] = rx_rd_ptr & 0xFF;
    if (W5500_Write_Bytes(spi_id, socket_bsb, W5500_Sn_RX_RD0, rx_rd_ptr_bytes, 2) != W5500_OK) {
        return W5500_ERROR;
    }

    // 5. 發出 RECV 命令通知資料已讀完，可釋放空間
    uint8_t socket_command = W5500_CR_RECV;
    if (W5500_Write_Byte(spi_id, socket_bsb, W5500_Sn_CR, &socket_command) != W5500_OK) {
        return W5500_ERROR;
    }

    // 6. 等待 CR 清除
    uint8_t cr;
    uint32_t timeout = 1000;
    do {
        if (W5500_Read_Byte(spi_id, socket_bsb, W5500_Sn_CR, &cr) != W5500_OK) {
            return W5500_ERROR;
        }
    } while (cr != 0x00 && --timeout > 0);
    if (timeout == 0) {
        return W5500_TIMEOUT;
    }

    return W5500_OK;
}

W5500_StatusTypeDef W5500_Socket_Get_Status(uint8_t spi_id, uint8_t socket_num, W5500_SocketTypeDef *socket, uint8_t *status) {
    if (socket_num >= W5500_MAX_SOCKETS) {
        return W5500_ERROR;
    }

    uint8_t socket_bsb = W5500_BSB_SOCKET_REG(socket_num);
    uint8_t sr;

    if (W5500_Read_Byte(spi_id, socket_bsb, W5500_Sn_SR, &sr) != W5500_OK) {
        return W5500_ERROR;
    }

    socket->state = sr;  // 更新內部狀態
    if (status != NULL) {
        *status = sr;     // 回傳狀態值給呼叫端
    }

    printf("Socket %d Sn_SR 狀態：0x%02X\n", socket_num, sr);

    return W5500_OK;
}

