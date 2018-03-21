#include "subLogic_sx1278.h"

SPI_HandleTypeDef *hspi1;

void sx1278_SetSPI(SPI_HandleTypeDef *spi) { hspi1 = spi; }

void sx1278_Reset(void) {
    HAL_GPIO_WritePin(RST_PORT_GPIO_PORT, RST_PORT_PIN, GPIO_PIN_RESET);
    DELAY_100us(); // min 100us
    HAL_GPIO_WritePin(RST_PORT_GPIO_PORT, RST_PORT_PIN, GPIO_PIN_SET);
    DELAY_nms(5); // min 5ms
}

void sx1278_Tx(void) {
    HAL_GPIO_WritePin(RX_GPIO_PORT, RX_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(TX_GPIO_PORT, TX_PIN, GPIO_PIN_RESET);
}

void sx1278_Rx(void) {
    HAL_GPIO_WritePin(RX_GPIO_PORT, RX_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(TX_GPIO_PORT, TX_PIN, GPIO_PIN_SET);
}

void sx1278_WriteByte(uint8_t addr, uint8_t data) {
    HAL_GPIO_WritePin(NSS_GPIO_PORT, NSS_PIN, GPIO_PIN_RESET);
    uint8_t buf = addr | 0x80;
    HAL_SPI_Transmit(hspi1, &buf, 1, 100000);
    HAL_SPI_Transmit(hspi1, &data, 1, 100000);
    HAL_GPIO_WritePin(NSS_GPIO_PORT, NSS_PIN, GPIO_PIN_SET);
}

void sx1278_WriteNbytes(uint8_t addr, uint8_t *buffer, uint8_t size) {
    HAL_GPIO_WritePin(NSS_GPIO_PORT, NSS_PIN, GPIO_PIN_RESET);

    uint8_t newAddr = addr | 0x80;
    HAL_SPI_Transmit(hspi1, &newAddr, 1, 100000);
    HAL_SPI_Transmit(hspi1, buffer, size, 1000000);
    HAL_GPIO_WritePin(NSS_GPIO_PORT, NSS_PIN, GPIO_PIN_SET);
}

void sx1278_ReadByte(uint8_t addr, uint8_t *buffer) {
    HAL_GPIO_WritePin(NSS_GPIO_PORT, NSS_PIN, GPIO_PIN_RESET);

    uint8_t newAddr = (addr & 0x7F);
    HAL_SPI_Transmit(hspi1, &newAddr, 1, 100000);
    HAL_SPI_Receive(hspi1, buffer, 1, 100000);
    HAL_GPIO_WritePin(NSS_GPIO_PORT, NSS_PIN, GPIO_PIN_SET);
}

void sx1278_ReadNbytes(uint8_t addr, uint8_t *buffer, uint8_t size) {
    HAL_GPIO_WritePin(NSS_GPIO_PORT, NSS_PIN, GPIO_PIN_RESET);
    uint8_t newAddr = (addr & 0x7F);
    HAL_SPI_Transmit(hspi1, &newAddr, 1, 100000);
    HAL_SPI_Receive(hspi1, buffer, size, 1000000);
    HAL_GPIO_WritePin(NSS_GPIO_PORT, NSS_PIN, GPIO_PIN_SET);
}

void sx1278_WriteRxTx(uint8_t enable) {
    if (enable)
        sx1278_Tx();
    else
        sx1278_Rx();
}
