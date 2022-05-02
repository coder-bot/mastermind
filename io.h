#ifndef IO_H
#define IO_H

#include "mm_color.h"

/* Print welcome banner */
void banner(void);

/* Print all available colors as ASCII and emoji, plus rules */
void print_rules(void);

/* Print a vector of SECRET_LEN mm_color objects as emoji */
void print_colors_as_emoji(const enum mm_color *cc);

/* Collect a guess. Returns enum mm_color [4]; returns NULL if there was
 * a low-level input error. */
enum mm_color *get_guess(void);

#endif /* IO_H */

