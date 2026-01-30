cmake_minimum_required(VERSION 3.15)
project(spi_eeprom)

set(CMAKE_CXX_STANDARD 20)

set(SRC
        src/Eeprom25LC040A.cpp
        src/NorFlashW25Q128.cpp
)

add_executable(test_eeprom
        tests/test_eeprom.cpp
        ${SRC}
)

target_include_directories(test_eeprom PRIVATE include)
