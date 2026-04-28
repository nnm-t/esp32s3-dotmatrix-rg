#pragma once

#include <Arduino.h>

#include <TM16xxMatrix.h>

#include "logic_74hc138.h"

enum class DotMatixColor : uint8_t
{
    None = 0x00,
    Red = 0x01,
    Green = 0x02,
    Orange = 0x03
};

class DotMatrix
{
    static constexpr const size_t matrix_color_columns = 8;
    static constexpr const size_t matrix_rows = 8;

    Logic74HC138& _logic;
    TM16xxMatrix** _matrix;
    const size_t _matrix_x_num;
    const size_t _matrix_y_num;

public:
    DotMatrix(Logic74HC138& logic, TM16xxMatrix** matrix, const size_t matrix_x_num, const size_t matrix_y_num) : _logic(logic), _matrix(matrix), _matrix_x_num(matrix_x_num), _matrix_y_num(matrix_y_num)
    {

    }
    const uint8_t put_char(const char* c, const DotMatixColor color = DotMatixColor::Red, const int32_t offset_x = 0);

    const size_t put_string(const char* string, const DotMatixColor color = DotMatixColor::Red, const int32_t offset_x = 0);

    void clear(const size_t col_begin, const size_t col_end);

    void clear_all();
};