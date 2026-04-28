#include "logic_74hc138.h"

void Logic74HC138::begin()
{
    pinMode(_select_a, OUTPUT);
    pinMode(_select_b, OUTPUT);
    pinMode(_select_c, OUTPUT);
}

void Logic74HC138::write(const uint8_t value_a, const uint8_t value_b, const uint8_t value_c)
{
    digitalWrite(_select_a, value_a);
    digitalWrite(_select_b, value_b);
    digitalWrite(_select_c, value_c);
}

void Logic74HC138::select(const uint8_t output)
{
    switch (output)
    {
        case 0:
            write(LOW, LOW, LOW);
            break;
        case 1:
            write(HIGH, LOW, LOW);
            break;
        case 2:
            write(LOW, HIGH, LOW);
            break;
        case 3:
            write(HIGH, HIGH, LOW);
            break;
        case 4:
            write(LOW, LOW, HIGH);
            break;
        case 5:
            write(HIGH, LOW, HIGH);
            break;
        case 6:
            write(LOW, HIGH, HIGH);
            break;
        case 7:
            write(HIGH, HIGH, HIGH);
            break;
        default:
            // write(LOW, LOW, LOW);
            break;
    }
}