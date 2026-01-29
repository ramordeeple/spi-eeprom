/**
 * @brief Интерфейс SPI драйвера
 *
 * Определяет минимальный набор операции для работы по SPI:
 * Управление сигналом CS и обмен байтами
 */

#pragma once
#include <cstdint>

class ISpiDriver {
public:
    /**
     * @brief Активировать микросхему (CS = 0)
     */
    virtual void select() = 0;

    /**
     * @brief Деактивировать микросхему (CS = 1)
     */
    virtual void deselect() = 0;

    /**
     * @brief Передать и принять 1 байт по SPI
     * @param data Байт для передачи
     * @return Принятый байт
     */
    virtual uint8_t transfer(uint8_t data) = 0;

    virtual ~ISpiDriver() = default;
};
