#include "../include/ISpiDriver.h"
#include "../include/Eeprom25LC040A.h"

Eeprom25LC040A::Eeprom25LC040A(ISpiDriver& spi) : spi_(spi) {}

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

uint8_t Eeprom25LC040A::readByte(uint32_t address) {
    spi_.select();
    spi_.transfer(static_cast<uint8_t>(Command::Read));
    spi_.transfer((address >> 8) & 0xFF);
    spi_.transfer(address & 0xFF);
    uint8_t value = spi_.transfer(0xFF); // Dummy-byte для чтения
    spi_.deselect();

    return value;
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

