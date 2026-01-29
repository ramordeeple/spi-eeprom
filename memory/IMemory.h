#pragma once
#include <cstdint>

class IMemory {
public:
    virtual bool read(uint32_t address, uint8_t* data, size_t) = 0;
    virtual bool write(uint32_t address, const uint8_t* data, size_t size) = 0;

    virtual bool readByte(uint32_t address, uint8_t& value) = 0;
    virtual bool writeByte(uint32_t address, uint8_t value) = 0;


    virtual ~IMemory() = default;
};

