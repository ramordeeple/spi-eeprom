/**
 * @brief Драйвер SPI NOR Flash Winbond W25Q128
 *
 * Отличия от EEPROM:
 * - перед записью требуется стирание сектора
 * - запись выполняется страницами
 * - больший адресный диапазон
 */

#pragma once

#include <cstdint>
#include <cstddef>

#include "IMemory.h"
#include "ISpiDriver.h"

class NorFlashW25Q128 : public IMemory {
public:
    /**
     * @brief Конструктор драйвера NOR Flash
     * @param spi Ссылка на SPI драйвер
     */
    explicit NorFlashW25Q128(ISpiDriver& spi);

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
    static constexpr size_t PageSize = 256; /**< Размер страницы памяти */

    // Команды SPI для NOR Flash
    enum class Command : uint8_t {
        WriteEnable = 0x06,
        Read        = 0x03,
        PageProgram = 0x02,
        ReadStatus  = 0x05,
        SectorErase = 0x20,
    };

    // Отправляет 24-битный адрес на SPI
    void sendAddress(uint32_t address);

    // Устанавливает флаг Write Enable
    void writeEnable();

    // Ожидает готовность памяти (WIP бит = 0)
    void waitReady();

    // Ссылка на SPI драйвер
    ISpiDriver& spi_;
};
