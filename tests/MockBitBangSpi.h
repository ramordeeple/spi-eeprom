/**
 * @brief Заглушка SPI драйвера
 * Нужен для компиляции и тестирования
 * без реального оборудования
*/

#pragma once
#include <vector>

#include "../include/ISpiDriver.h"

using namespace std;

class MockBitBangSpi : public ISpiDriver {
public:
    /** @brief Имитирует выбор устройства на шине SPI*/
    void select() override;

    /** @brief Имитирует снятие выбора устройства на шине SPI */
    void deselect() override;

    /** @brief Имитирует отправку/приём байта по SPI */
    uint8_t transfer(uint8_t data) override {
        (void) data;

        return 0xFF;
    }

    /** @brief Очистка лога SPI операций */
    void clearLog();

    /**
     * @brief Получение лога SPI операций
     * @return Константная ссылка на вектор логов
     */
    const vector<uint8_t>& getLog() const;

private:
    vector<uint8_t> log; // Лог всех SPI операций
};
