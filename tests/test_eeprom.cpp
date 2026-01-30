/**
 * @brief Тест драйвера 25LC040A
 *
 * Проверяет базовую работу методов writeByte/readByte с SPI-заглушкой
 */

#include <cassert>
#include <iostream>
#include <ostream>
#include <_stdlib.h>

#include "../include/Eeprom25LC040A.h"
#include "MockBitBangSpi.h"

using namespace std;

MockBitBangSpi spi;
Eeprom25LC040A eeprom(spi);

void setup();
void testWriteByte();
void testReadByte();
void testWriteReadArray();

int main() {
    testWriteByte();
    testReadByte();
    testWriteReadArray();

    cout << endl << "ALL EEPROM TESTS COMPLETED SUCCESSFULLY" << endl << endl;
    return EXIT_SUCCESS;
}

/** @brief Настройка перед каждым тестом */
void setup() {
    spi.clearLog();
}

/** @brief Тест записи 1 байта */
void testWriteByte() {
    setup();

    uint32_t addr = 0x10;
    uint8_t value = 0xAB;

    bool ok = eeprom.writeByte(addr, value);
    assert(ok);

    cout << "SPI Log after writeByte: " << endl;
    for (auto b : spi.getLog())
        cout << hex << static_cast<int>(b) << endl << endl;

}

/** @brief Тест чтения 1 байта */
void testReadByte() {
    setup();

    uint32_t addr = 0x10;
    uint8_t value = 0;

    bool ok = eeprom.readByte(addr, value);
    assert(ok);

    cout << "SPI Log after readByte: " << endl;

    for (auto b : spi.getLog())
        cout << hex << static_cast<int>(b) << endl << endl;

}

/** @brief Тест записи и чтения массива байт */
void testWriteReadArray() {
    setup();
    static constexpr size_t ArraySize = 4;

    uint32_t addr = 0x20;
    uint8_t dataWrite[ArraySize] = {0x11, 0x22, 0x33, 0x44};
    uint8_t dataRead[ArraySize] = {0};

    bool ok = eeprom.write(addr, dataWrite, ArraySize);
    assert(ok);

    ok = eeprom.read(addr, dataRead, ArraySize);
    assert(ok);

    cout << "SPI Log after writeReadArray: " << endl;
    for (auto b : spi.getLog())
        cout << hex << static_cast<int>(b) << endl << endl;

}

