#pragma once
#include <cstdint>

class IMemory {
public:
    /**
     * @brief Чтение массива байт
     */
    virtual bool read(uint32_t address, uint8_t *data, size_t) = 0;

    /**
    * @brief Запись массива байт
    */
    virtual bool write(uint32_t address, const uint8_t *data, size_t size) = 0;

    /**
    * @brief Чтение 1 байта
    */
    virtual bool readByte(uint32_t address, uint8_t &value) = 0;

    /**
    * @brief Запись 1 байта
    */
    virtual bool writeByte(uint32_t address, uint8_t value) = 0;

    virtual ~IMemory() = default;
};
