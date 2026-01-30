/**
 * @brief Интерфейс энергенезависимой памяти,
 * т.е. данные сохраняются при отключении электропитания
 */

#pragma once
#include <cstddef>
#include <cstdint>

class IMemory {
public:
    /**
     * @brief Читает один бит из памяти
     * @param address Адрес байта
     * @param bitPosition Позиция бита (0-7)
     * @param value Считанное значение бита
     * @return true, если операция успешна
     */
    virtual bool readBit(uint32_t address, uint8_t bitPosition, bool &value) = 0;

    /**
     * @brief Записывает один бит в память
     * @param address Адрес байта
     * @param bitPosition Позиция бита (0-7)
     * @param value Значение бита для записи
     * @return true, если операция успешна
     */
    virtual bool writeBit(uint32_t address, uint8_t bitPosition, bool value) = 0;

    /**
     * @brief Читает массив байт из памяти
     * @param address Адрес начала чтения
     * @param data Буфер для чтения
     * @param size Количество байт
     * @return true, если операция успешна
     */
    virtual bool read(uint32_t address, uint8_t *data, size_t size) = 0;

    /**
     * @brief Записывает массив байт в память
     * @param address Адрес начала записи
     * @param data Буфер с данными
     * @param size Количество байт
     * @return true, если операция успешна
     */
    virtual bool write(uint32_t address, const uint8_t *data, size_t size) = 0;

    /**
     * @brief Читает 1 байт из памяти
     * @param address Адрес для чтения
     * @param value Считанное значение
     * @return true, если операция успешна
     */
    virtual bool readByte(uint32_t address, uint8_t &value) = 0;

    /**
     * @brief Записывает 1 байт в память
     * @param address Адрес для записи
     * @param value Значение байта
     * @return true, если операция успешна
     */
    virtual bool writeByte(uint32_t address, uint8_t value) = 0;

    virtual ~IMemory() = default;
};
