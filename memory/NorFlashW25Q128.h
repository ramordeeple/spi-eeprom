/**
 * @brief Драйвер SPI NOR Flash Winbond W25Q128
 *
 * Отличия от EEPROM:
 * - перед записью требуется стирание сектора
 * - запись выполняется страницами
 * - больший адресный диапазон
 */

#pragma once
#include "IMemory.h"
#include "../spi/ISpiDriver.h"

class NorFlashW25Q128 : public IMemory {
public:
    explicit NorFlashW25Q128(ISpiDriver& spi) : spi_(spi) {}

    bool read(uint32_t address, uint8_t *data, size_t size) override {
        spi_.select();
        spi_.transfer(static_cast<uint8_t>(Command::Read));
        sendAddress(address);

        for (size_t i = 0; i < size; ++i) {
            data[i] = spi_.transfer(0xFF);
        }
    }

private:
    static constexpr size_t PageSize = 256;

    enum class Command : uint8_t {
        WriteEnable = 0x06,
        Read        = 0x03,
        PageProgram = 0x02,
        ReadStatus  = 0x05,
        SectorErase = 0x20,
    };

    void sendAddress(uint32_t address) {
        spi_.transfer((address >> 16) & 0xFF);
        spi_.transfer((address >> 8) & 0xFF);
        spi_.transfer(address & 0xFF);
    }

    void writeEnable() {
        spi_.select();
        spi_.transfer(static_cast<uint8_t>(Command::WriteEnable));
        spi_.deselect();
    }

    void waitReady() {
        spi_.select();
        spi_.transfer(static_cast<uint8_t>(Command::ReadStatus));

        while (spi_.transfer(0xFF) & 0x01) {}
        spi_.deselect();
    }

    ISpiDriver& spi_;
};
