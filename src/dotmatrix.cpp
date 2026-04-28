#include <efont.h>
#include <efontEnableJa.h>

#include "dotmatrix.h"

const uint8_t DotMatrix::put_char(const char* ch, const DotMatixColor color, const int32_t offset_x)
{
    // 文字コード取得
    uint16_t str_utf16;
    efontUFT8toUTF16(&str_utf16, const_cast<char*>(ch));

    Serial.printf("str: U+%04X\n", str_utf16);

    // フォントデータ取得
    // 16 x 16 px: (8 * 2) * 16 bytes
    uint8_t font[32];
    getefontData(font, str_utf16);

    // 全角文字と半角文字
    const uint8_t width = str_utf16 < 0x0100 ? 8 : 16;

    for (uint8_t row = 0; row < 16; row++)
    {
        const uint16_t font_data = (font[row * 2] << 8) + font[row * 2 + 1];

        for (uint8_t col = 0; col < width; col++)
        {
            // オフセット
            int32_t x = col + offset_x;
            if (x < 0 || x > matrix_color_columns * _matrix_x_num - 1)
            {
                continue;
            }

            const uint8_t select = (x / 8) * _matrix_y_num + (row / 8);
            if (select > _matrix_x_num * _matrix_y_num - 1)
            {
                continue;
            }

            Serial.print((0x8000 >> x) & font_data ? "#" : " ");

            _logic.select(select);

            // 赤、緑 点灯
            bool is_red_on = false;
            bool is_green_on = false;
            switch (color)
            {
                case DotMatixColor::Red:
                    is_red_on = (0x8000 >> col) & font_data;
                    is_green_on = false;
                    break;
                case DotMatixColor::Green:
                    is_red_on = false;
                    is_green_on = (0x8000 >> col) & font_data;
                    break;
                case DotMatixColor::Orange:
                    is_red_on = (0x8000 >> col) & font_data;
                    is_green_on = (0x8000 >> col) & font_data;
                    break;
                default:
                    is_red_on = false;
                    is_green_on = false;
                    break;
            }
            _matrix[select]->setPixel(x % 8, row % 8, is_red_on);
            _matrix[select]->setPixel((x % 8) + 8, row % 8, is_green_on);
        }

        Serial.print("\n");
    }

    return width;
}

const size_t DotMatrix::put_string(const char* string, const DotMatixColor color, const int32_t offset_x)
{
    // UTF-8文字列を1文字ずつ切り出し
    size_t cursor = 0;
    size_t str_width = 0;

    while (string[cursor] != '\0')
    {
        char ch[5] = { 0 };
        // 条件に当てはまらない文字は1byte
        uint8_t utf8_length = 1;
        if (string[cursor] & 0xF0)
        {
            // 4bytes
            utf8_length = 4;
        }
        if (string[cursor] & 0xE0)
        {
            // 3bytes
            utf8_length = 3;
        }
        else if (string[cursor] & 0xC0)
        {
            // 2bytes
            utf8_length = 2;
        }

        // 1文字切り出し
        memcpy(ch, string + cursor, utf8_length);
        str_width += put_char(ch, color, offset_x + str_width);

        // カーソル移動
        cursor += utf8_length;
    }

    return str_width;
}

void DotMatrix::clear(const size_t col_begin, const size_t col_end)
{
    for (size_t x = col_begin; x <= col_end; x++)
    {
        if (x < 0 || x > _matrix_x_num * matrix_color_columns - 1)
        {
            continue;
        }

        const size_t select_begin = (x / 8) * _matrix_y_num;
        for (size_t i = select_begin; i < select_begin + _matrix_y_num; i++)
        {
            _logic.select(i);

            for (size_t y = 0; y < matrix_rows; y++)
            {
                _matrix[i]->setPixel(x % 8, y, false);
                _matrix[i]->setPixel((x % 8) + 8, y, false);
            }
        }
    }
}

void DotMatrix::clear_all()
{
    for (size_t i = 0; i < _matrix_x_num * _matrix_y_num; i++)
    {
        _logic.select(i);
        _matrix[i]->setAll(false);
    }
}