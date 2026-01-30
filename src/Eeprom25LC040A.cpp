#include "../include/ISpiDriver.h"
#include "../include/Eeprom25LC040A.h"

#include <ranges>

using namespace std;

const uint32_t MEMORY_SIZE = 512;

Eeprom25LC040A::Eeprom25LC040A(ISpiDriver& spi) : spi_(spi) {}

bool Eeprom25LC040A::read(uint32_t address, uint8_t *data, size_t size) {
    if (data == nullptr || size == 0)
        return false;

    if (address >= MEMORY_SIZE || (address + size) > MEMORY_SIZE)
        return false;

    spi_.select();
    spi_.transfer(static_cast<uint8_t>(Command::Read));
    spi_.transfer(static_cast<uint8_t>((address >> 8) & 0xFF));
    spi_.transfer(static_cast<uint8_t>(size & 0xFF));

    for (size_t i = 0; i < size; ++i)
        data[i] = spi_.transfer(0xFF);

    spi_.deselect();

    return true;
}

bool Eeprom25LC040A::write(uint32_t address, const uint8_t* data, size_t size) {
    if (data == nullptr || size == 0)
        return false;

    if (address >= MEMORY_SIZE || (address + size) > MEMORY_SIZE)
        return false;

    const size_t PAGE_SIZE = 16;
    size_t bytesWritten = 0;

    while (bytesWritten < size) {
        uint32_t currentAddress = address + bytesWritten;
        size_t pageOffset = currentAddress % PAGE_SIZE;
        size_t chunkSize = min(size - bytesWritten, PAGE_SIZE - pageOffset);

        writeEnable();

        spi_.select();
        spi_.transfer(static_cast<uint8_t>(Command::Write));
        spi_.transfer(static_cast<uint8_t>((currentAddress >> 8) & 0xFF));
        spi_.transfer(static_cast<uint8_t>(currentAddress & 0xFF));

        for (size_t i = 0; i < chunkSize; ++i)
            spi_.transfer(data[bytesWritten + i]);

        spi_.deselect();
        waitWriteComplete();

        bytesWritten += chunkSize;
    }

    return true;
}

bool Eeprom25LC040A::writeByte(uint32_t address, uint8_t value) {
    writeEnable();          // Разрешение на запись
    spi_.select();
    spi_.transfer(static_cast<uint8_t>(Command::Write));
    spi_.transfer((address >> 8) & 0xFF);
    spi_.transfer(address & 0xFF);
    spi_.transfer(value);
    spi_.deselect();
    waitWriteComplete();    // Ожидание окончания записи

    return true;
}

bool Eeprom25LC040A::readByte(uint32_t address, uint8_t& value) {
    spi_.select();
    spi_.transfer(static_cast<uint8_t>(Command::Read));
    spi_.transfer((address >> 8) & 0xFF);
    spi_.transfer(address & 0xFF);

    value = spi_.transfer(0xFF); // Dummy-byte для чтения
    spi_.deselect();

    return true;
}

void Eeprom25LC040A::writeEnable() {
    spi_.select();
    spi_.transfer(static_cast<uint8_t>(Command::WriteEnable));
    spi_.deselect();
}

void Eeprom25LC040A::waitWriteComplete() {
    uint8_t status;
    do {
        spi_.select();
        spi_.transfer(static_cast<uint8_t>(Command::ReadStatus));
        status = spi_.transfer(0xFF);
        spi_.deselect();
    } while(status & 0x01); // WIP (Write-In-Progress) бит
}

