/**
 * @brief Драйвер SPI EEPROM Microchip 25LC040A
 *
 * Особенности:
 * - запись возможна по байтам
 * - требуется команда Write Enable
 * - необходимо ожидание завершения записи
 */

#pragma once
#include "IMemory.h"
#include "../spi/ISpiDriver.h"

class Eeprom25LC040A : public IMemory {
public:
    explicit Eeprom25LC040A(ISpiDriver &spi) : spi_(spi) {}

    bool read(uint32_t address, uint8_t *data, size_t size) override {
        spi_.select();
        spi_.transfer(static_cast<uint8_t>(Command::Read));
        spi_.transfer(static_cast<uint8_t>(address));

        for (size_t i = 0; i < size; ++i)
            data[i] = spi_.transfer(0xFF);

        spi_.deselect();

        return true;
    }

    bool write(uint32_t address, const uint8_t *data, size_t size) override {
        for (size_t i = 0; i < size; ++i)
            writeByte(address + i, data[i]);

        return true;
    }

    bool readByte(uint32_t address, uint8_t &value) override {
        return read(address, &value, 1);
    }

    bool writeByte(uint32_t address, uint8_t value) override {
        writeEnable();
        spi_.select();
        spi_.transfer(static_cast<uint8_t>(Command::Write));
        spi_.transfer(static_cast<uint8_t>(address));
        spi_.transfer(value);
        spi_.deselect();
        waitWriteComplete();

        return true;
    }

private:
    enum class Command : uint8_t {
        WriteEnable = 0x06,
        Write       = 0x02,
        Read        = 0x03,
        ReadStatus  = 0x05,
    };

    void writeEnable() {
        spi_.select();
        spi_.transfer(static_cast<uint8_t>(Command::WriteEnable));
        spi_.deselect();
    }

    void waitWriteComplete() {
        spi_.select();
        spi_.transfer(static_cast<uint8_t>(Command::ReadStatus));

        while (spi_.transfer(0xFF) & 0x01) {}

        spi_.deselect();
    }

    ISpiDriver& spi_;
};
