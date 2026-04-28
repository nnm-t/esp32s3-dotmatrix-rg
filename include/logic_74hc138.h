#pragma once

#include <Arduino.h>

class Logic74HC138
{
    const size_t _select_a;
    const size_t _select_b;
    const size_t _select_c;

    void write(const uint8_t value_a, const uint8_t value_b, const uint8_t value_c);

public:
    Logic74HC138(const size_t select_a, const size_t select_b, const size_t select_c) : _select_a(select_a), _select_b(select_b), _select_c(select_c)
    {

    }

    void begin();

    void select(const uint8_t output);
};