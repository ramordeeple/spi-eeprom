#include "../include/NorFlashW25Q128.h"
#include <algorithm>

using namespace std;

NorFlashW25Q128::NorFlashW25Q128(ISpiDriver& spi) : spi_(spi) {}

bool NorFlashW25Q128::read(uint32_t address, uint8_t *data, size_t size) {
    spi_.select();
    spi_.transfer(static_cast<uint8_t>(Command::Read));
    sendAddress(address);

    for (size_t i = 0; i < size; ++i)
        data[i] = spi_.transfer(0xFF); // dummy byte

    spi_.deselect();

    return true;
}

bool NorFlashW25Q128::write(uint32_t address, const uint8_t *data, size_t size) {
    size_t written = 0;

    while (written < size) {
        writeEnable();

        spi_.select();
        spi_.transfer(static_cast<uint8_t>(Command::PageProgram));
        sendAddress(address + written);

        size_t pageBytes = min(PageSize - ((address + written) % PageSize), size - written);

        for (size_t i = 0; i < pageBytes; ++i)
            spi_.transfer(data[written + i]);

        spi_.deselect();
        waitReady();

        written += pageBytes;
    }

    return true;
}

bool NorFlashW25Q128::readByte(uint32_t address, uint8_t& value) {
    return read(address, &value, 1);
}

bool NorFlashW25Q128::writeByte(uint32_t address, uint8_t value) {
    return write(address, &value, 1);
}

void NorFlashW25Q128::sendAddress(uint32_t address) {
    spi_.transfer((address >> 16) & 0xFF);
    spi_.transfer((address >> 8) & 0xFF);
    spi_.transfer(address & 0xFF);
}

void NorFlashW25Q128::writeEnable() {
    spi_.select();
    spi_.transfer(static_cast<uint8_t>(Command::WriteEnable));
    spi_.deselect();
}

void NorFlashW25Q128::waitReady() {
    spi_.select();
    spi_.transfer(static_cast<uint8_t>(Command::ReadStatus));

    while (spi_.transfer(0xFF) != 0x01) {} // WIP бит
    spi_.deselect();
}
