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
    void select() override {
        log.push_back(0xAA);
    }

    /** @brief Имитирует снятие выбора устройства на шине SPI */
    void deselect() override {
        log.push_back(0xBB);
    }

    /** @brief Имитирует отправку/приём байта по SPI */
    uint8_t transfer(uint8_t data) override {
        log.push_back(data);

        return 0xFF;
    }

    /** @brief Очистка лога SPI операций */
    void clearLog() {
        log.clear();
    }

    /**
     * @brief Получение лога SPI операций
     * @return Константная ссылка на вектор логов
     */
    const vector<uint8_t>& getLog() const {
        return log;
    }

private:
    vector<uint8_t> log; // Лог всех SPI операций
};
