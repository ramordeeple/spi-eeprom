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
#include "ISpiDriver.h"
#include <cstdint>
#include <cstddef>

class Eeprom25LC040A : public IMemory {
public:
    /**
     * @brief Конструктор драйвера EEPROM
     * @param spi SPI-интерфейс для взаимодействия с микросхемой
     */
    explicit Eeprom25LC040A(ISpiDriver& spi);

    /**
     * @brief Читает массив байт из памяти
     * @param address Адрес начала чтения
     * @param data Буфер для чтения
     * @param size Количество байт
     * @return true, если операция успешна
     */
    bool read(uint32_t address, uint8_t* data, size_t size) override;

    /**
     * @brief Записывает массив байт в память
     * @param address Адрес начала записи
     * @param data Буфер с данными
     * @param size Количество байт
     * @return true, если операция успешна
     */
    bool write(uint32_t address, const uint8_t* data, size_t size) override;

    /**
     * @brief Читает один байт из памяти
     * @param address Адрес для чтения
     * @param value Считанное значение
     * @return true, если операция успешна
     */
    bool readByte(uint32_t address, uint8_t& value) override;

    /**
     * @brief Записывает один байт в память
     * @param address Адрес для записи
     * @param value Значение байта
     * @return true, если операция успешна
     */
    bool writeByte(uint32_t address, uint8_t value) override;

private:
    // Команды SPI EEPROM
    enum class Command : uint8_t {
        WriteEnable = 0x06,
        Write       = 0x02,
        Read        = 0x03,
        ReadStatus  = 0x05,
    };

    // Устанавливает Write Enable
    void writeEnable();

    // Ждёт окончания операции записи
    void waitWriteComplete();

    // Ссылка на SPI драйвер
    ISpiDriver& spi_;
};
