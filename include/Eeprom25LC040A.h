/**
  * @brief Драйвер SPI EEPROM Microchip 25LC040A
  * Особенности:
  * - запись возможна по байтам
  * - требуется команда Write Enable
  * - необходимо ожидание завершения записи
  */

#pragma once

#include <cstdint>
#include <cstddef>

#include "IMemory.h"
#include "ISpiDriver.h"

class Eeprom25LC040A : public IMemory {
public:
    /**
     * @brief Конструктор драйвера EEPROM
     * @param spi SPI-интерфейс для взаимодействия с микросхемой
     */
    explicit Eeprom25LC040A(ISpiDriver &spi);

    /**
     * @see IMemory::readBit()
     */
    bool readBit(uint32_t address, uint8_t bitPosition, bool &value) override;

    /**
     * @see IMemory::writeBit()
     */
    bool writeBit(uint32_t address, uint8_t bitPosition, bool value) override;

    /**
     * @see IMemory::read()
     */
    bool read(uint32_t address, uint8_t *data, size_t size) override;

    /**
     * @see IMemory::write()
     */
    bool write(uint32_t address, const uint8_t *data, size_t size) override;

    /**
     * @see IMemory::readByte()
     */
    bool readByte(uint32_t address, uint8_t &value) override;

    /**
     * @see IMemory::writeByte()
     */
    bool writeByte(uint32_t address, uint8_t value) override;

private:
    // Команды SPI EEPROM
    enum class Command : uint8_t {
        WriteEnable = 0x06,
        Write = 0x02,
        Read = 0x03,
        ReadStatus = 0x05,
    };

    // Устанавливает Write Enable
    void writeEnable();

    // Ждёт окончания операции записи
    void waitWriteComplete();

    // Ссылка на SPI драйвер
    ISpiDriver &spi_;
};
