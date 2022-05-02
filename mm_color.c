#include "mm_color.h"

int ascii_is_mm_color(const char c)
{
    return ascii_to_mm_color(c) != NOT_A_COLOR;
}

enum mm_color ascii_to_mm_color(const char c)
{
    int i;
    for (i = 0; i < MM_COLOR_TOTAL; i++) {
        if (c == mm_color_ascii[i]) return i;
    }

    return NOT_A_COLOR;
}

char mm_color_to_ascii(const enum mm_color c)
{

    if (c >= 0 && c < MM_COLOR_TOTAL) {
        return mm_color_ascii[c];
    } else {
        return '?';
    }
}

char *mm_color_to_emoji(const enum mm_color c)
{
    if (c >= 0 && c < MM_COLOR_TOTAL) {
        return mm_color_emoji[c];
    } else {
        return "?";
    }
}

