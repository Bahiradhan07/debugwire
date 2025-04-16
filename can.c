#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define SPI_DEVICE "/dev/spidev0.0"
#define SPI_SPEED  10000000  // 10 MHz
#define SPI_MODE   0

// MCP2518FD SPI Commands
#define CMD_RESET  0xC0
#define CMD_READ   0x03
#define CMD_WRITE  0x02

// Example Register Address
#define OSC_REGISTER  0xE00  // Oscillator Control Register

int spi_fd;

// SPI Initialization
int spi_init() {
    spi_fd = open(SPI_DEVICE, O_RDWR);
    if (spi_fd < 0) {
        perror("SPI Open Failed");
        return -1;
    }

    uint8_t mode = SPI_MODE;
    uint32_t speed = SPI_SPEED;

    if (ioctl(spi_fd, SPI_IOC_WR_MODE, &mode) < 0) {
        perror("SPI Mode Set Failed");
        return -1;
    }

    if (ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
        perror("SPI Speed Set Failed");
        return -1;
    }

    return 0;
}

// SPI Transfer Function
int spi_transfer(uint8_t *tx, uint8_t *rx, int length) {
    struct spi_ioc_transfer spi_transfer = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = length,
        .speed_hz = SPI_SPEED,
        .bits_per_word = 8,
        .delay_usecs = 0
    };

    return ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi_transfer);
}

// Reset MCP2518FD
void reset_mcp2518fd() {
    uint8_t reset_cmd = CMD_RESET;
    write(spi_fd, &reset_cmd, 1);
    usleep(10000); // 10ms delay for reset
}

// Read Register
uint16_t read_register(uint16_t addr) {
    uint8_t tx_buf[4] = {CMD_READ, (addr >> 8) & 0xFF, addr & 0xFF, 0x00};
    uint8_t rx_buf[4] = {0};

    spi_transfer(tx_buf, rx_buf, 4);
    return (rx_buf[3] << 8) | rx_buf[2];
}

// Write Register
void write_register(uint16_t addr, uint16_t data) {
    uint8_t tx_buf[4] = {CMD_WRITE, (addr >> 8) & 0xFF, addr & 0xFF, data & 0xFF};
    spi_transfer(tx_buf, NULL, 4);
}

int main() {
    if (spi_init() < 0) {
        return -1;
    }

    printf("Resetting MCP2518FD...\n");
    reset_mcp2518fd();

    usleep(100000); // 100ms delay

    // Read oscillator register
    uint16_t osc_value = read_register(OSC_REGISTER);
    printf("Oscillator Register: 0x%04X\n", osc_value);

    // Modify oscillator value (example)
    write_register(OSC_REGISTER, osc_value | 0x04);
    printf("Updated Oscillator Register.\n");

    close(spi_fd);
    return 0;
}
