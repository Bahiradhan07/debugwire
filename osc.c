#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define SPI_DEVICE "/dev/spidev0.0"
#define SPI_SPEED  10000000
#define OSC_REGISTER  0xE00  // Oscillator Control Register
#define CMD_READ  0x03

int spi_fd;

// SPI Initialization
int spi_init() {
    spi_fd = open(SPI_DEVICE, O_RDWR);
    if (spi_fd < 0) {
        perror("SPI Open Failed");
        return -1;
    }
    uint32_t speed = SPI_SPEED;
    ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    return 0;
}

// Read Register
uint16_t read_register(uint16_t addr) {
    uint8_t tx_buf[4] = {CMD_READ, (addr >> 8) & 0xFF, addr & 0xFF, 0x00};
    uint8_t rx_buf[4] = {0};

    struct spi_ioc_transfer spi_transfer = {
        .tx_buf = (unsigned long)tx_buf,
        .rx_buf = (unsigned long)rx_buf,
        .len = 4,
        .speed_hz = SPI_SPEED,
        .bits_per_word = 8
    };

    ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi_transfer);
    return (rx_buf[3] << 8) | rx_buf[2];
}

int main() {
    if (spi_init() < 0) return -1;

    uint16_t osc_value = read_register(OSC_REGISTER);
    printf("OSC_REGISTER Value: 0x%04X\n", osc_value);

    close(spi_fd);
    return 0;
}
