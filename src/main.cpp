#include <Arduino.h>

#include <TM1640.h>
#include <TM16xxMatrix.h>

#include "dotmatrix.h"
#include "logic_74hc138.h"

namespace {
    constexpr const size_t tm1640_din = GPIO_NUM_4;
    constexpr const size_t tm1640_clk_en = GPIO_NUM_5;

    constexpr const size_t tm1640_clk_a = GPIO_NUM_10;
    constexpr const size_t tm1640_clk_b = GPIO_NUM_13;
    constexpr const size_t tm1640_clk_c = GPIO_NUM_14;

    constexpr const size_t matrix_columns = 16;
    constexpr const size_t matrix_rows = 8;
    constexpr const size_t matrix_x_num = 2;
    constexpr const size_t matrix_y_num = 2;
    constexpr const size_t matrix_num = matrix_x_num * matrix_y_num;

    TM1640 tm1640_0(tm1640_din, tm1640_clk_en);
    TM16xxMatrix matrix_0(&tm1640_0, matrix_columns, matrix_rows);

    TM1640 tm1640_1(tm1640_din, tm1640_clk_en);
    TM16xxMatrix matrix_1(&tm1640_1, matrix_columns, matrix_rows);

    TM1640 tm1640_2(tm1640_din, tm1640_clk_en);
    TM16xxMatrix matrix_2(&tm1640_2, matrix_columns, matrix_rows);

    TM1640 tm1640_3(tm1640_din, tm1640_clk_en);
    TM16xxMatrix matrix_3(&tm1640_3, matrix_columns, matrix_rows);

    TM1640* p_tm1640[matrix_num] = { &tm1640_0, &tm1640_1, &tm1640_2, &tm1640_3 };
    TM16xxMatrix* p_matrix[matrix_num] = { &matrix_0, &matrix_1, &matrix_2, &matrix_3 };

    Logic74HC138 logic_74hc138(tm1640_clk_a, tm1640_clk_b, tm1640_clk_c);

    DotMatrix dot_matrix(logic_74hc138, p_matrix, matrix_x_num, matrix_y_num);

    uint8_t led_status = LOW;
}

void setup()
{
    pinMode(21, OUTPUT);

    Serial.begin(115200);
    logic_74hc138.begin();

    for (uint8_t i = 0; i < matrix_num; i++)
    {
        logic_74hc138.select(i);

        delay(1);

        p_tm1640[i]->clearDisplay();
        p_matrix[i]->setAll(true);
        delay(100);
        p_matrix[i]->setAll(false);
        delay(100);
    }

    digitalWrite(21, led_status);

    delay(1000);
}

void loop()
{
    for (int32_t offset_x = 15; offset_x > -80; offset_x--)
    {
        if (offset_x > 0)
        {
            dot_matrix.clear(0, offset_x);
        }
        else if (offset_x < -64)
        {
            dot_matrix.clear(offset_x + 80, 15);
        }

        dot_matrix.put_string("普通　", DotMatixColor::Green, offset_x);
        dot_matrix.put_string("上野", DotMatixColor::Orange, offset_x + 48);

        led_status = led_status == LOW ? HIGH : LOW;
        digitalWrite(21, led_status);
    }
}