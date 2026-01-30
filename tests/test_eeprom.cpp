/**
 * @brief Тест драйвера 25LC040A
 *
 * Проверяет базовую работу методов writeByte/readByte с SPI-заглушкой
 */

#include <cassert>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <_stdlib.h>

#include "../include/Eeprom25LC040A.h"
#include "MockBitBangSpi.h"

using namespace std;

MockBitBangSpi spi;
Eeprom25LC040A eeprom(spi);

void setup();
void printLog(const string&);
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

/** @brief Вывод лога SPI операций */
void printLog(const string& testName) {
    cout << "[" << testName << "] SPI Operations:" << endl;
    const auto& log = spi.getLog();

    for (size_t i = 0; i < log.size(); ++i) {
        uint8_t b = log[i];

        // Расшифровка специальных маркеров
        if (b == 0xAA) {
            cout << "  -> SELECT" << endl;
        } else if (b == 0xBB) {
            cout << "  -> DESELECT" << endl;
        } else {
            cout << "  -> TRANSFER: 0x" << hex << uppercase
                 << setw(2) << setfill('0') << static_cast<int>(b) << dec << endl;
        }
    }
    cout << "  Total operations: " << log.size() << endl << endl;
}

/** @brief Тест записи 1 байта */
void testWriteByte() {
    setup();

    uint32_t addr = 0x10;
    uint8_t value = 0xAB;

    bool ok = eeprom.writeByte(addr, value);
    assert(ok && "writeByte failed");

    printLog("testWriteByte");
}

/** @brief Тест чтения 1 байта */
void testReadByte() {
    setup();

    uint32_t addr = 0x10;
    uint8_t value = 0;

    bool ok = eeprom.readByte(addr, value);
    assert(ok && "readByte failed");

    cout << "Read value: 0x" << hex << uppercase
         << setw(2) << setfill('0') << static_cast<int>(value) << dec << endl;

    printLog("testReadByte");
}

/** @brief Тест записи и чтения массива байт */
void testWriteReadArray() {
    setup();
    static constexpr size_t ArraySize = 4;

    uint32_t addr = 0x20;
    uint8_t dataWrite[ArraySize] = {0x11, 0x22, 0x33, 0x44};
    uint8_t dataRead[ArraySize] = {0};

    bool ok = eeprom.write(addr, dataWrite, ArraySize);
    assert(ok && "write failed");

    ok = eeprom.read(addr, dataRead, ArraySize);
    assert(ok && "read failed");

    cout << "Written: ";
    for (size_t i = 0; i < ArraySize; ++i) {
        cout << "0x" << hex << uppercase << setw(2) << setfill('0')
             << static_cast<int>(dataWrite[i]) << " ";
    }
    cout << dec << endl;

    printLog("testWriteReadArray");
}

