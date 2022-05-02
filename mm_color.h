#ifndef MM_COLOR_H
#define MM_COLOR_H

enum mm_color {
    NOT_A_COLOR = -1,
    PURPLE = 0,
    BLACK,
    RED,
    YELLOW,
    BLUE,
    GREEN,
    MM_COLOR_TOTAL
};

static char mm_color_ascii[] = {
    'P',
    'K',
    'R',
    'Y',
    'B',
    'G'
};

static char *mm_color_emoji[] = {
    "💜",
    "🖤",
    "❤️",
    "💛",
    "💙",
    "💚"
};

/* Validate whether an ASCII character c corresponds to an mm_color.
 * 1 for yes, 0 for no. */
int ascii_is_mm_color(char c);

/* Convert an ASCII character representation of a color c to its
 * mm_color equivalent; returns NOT_A_COLOR if c is not in the domain. */
enum mm_color ascii_to_mm_color(char c);

/* Convert mm_color c to an ASCII character representation; returns '?'
 * if c is out of range */
char mm_color_to_ascii(enum mm_color c);

/* Convert mm_color c to a color emoji heart; returns "?" if c is out of
 * range */
char *mm_color_to_emoji(enum mm_color c);

#endif /* MM_COLOR_H */

