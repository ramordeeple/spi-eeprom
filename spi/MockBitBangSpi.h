/**
 * @brief Заглушка SPI драйвера
 * Нужен для компиляции и тестирования
 * без реального оборудования
*/

#pragma once
#include "ISpiDriver.h"

class MockBitBangSpi : public ISpiDriver {
public:
    void select() override;
    void deselect() override;

    uint8_t transfer(uint8_t data) override {
        (void)data;

        return 0xFF;
    }
};
