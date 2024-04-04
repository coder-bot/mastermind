#include "io.h"

#include "mm_color.h"
#include "round.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Buffer length for reading an ASCII guess:
 * SECRET_LEN + room for 1 extra character + room for a null terminator.
 * See more details in the get_guess function. */
#define ASCII_BUFLEN (SECRET_LEN + 1 + 1)

void banner(void)
{
    printf("\n");
    printf("MASTER\n");
    printf(" MIND\n");
    printf("\n");
}

void print_rules(void)
{
    int i;

    for (i = 0; i < MM_COLOR_TOTAL; i++) {
        printf("%s : %c", mm_color_emoji[i], mm_color_ascii[i]);
        if (i != MM_COLOR_TOTAL - 1) {
            printf(", ");
        }
    }
    printf("\n");

    printf("The secret is %d colors.\n", SECRET_LEN);
    printf("You have %u guesses.\n", MAX_TURNS);

    printf("\n");
}

void print_colors_as_emoji(const enum mm_color *const cc)
{
    int i;
    for (i = 0; i < SECRET_LEN; i++) {
        printf("%s", mm_color_to_emoji(cc[i]));
        printf(" ");
    }
}

enum mm_color *get_guess(void)
{
    /* For reading and returning the guess, respectively. */
    char guess_ascii[ASCII_BUFLEN];
    static enum mm_color guess_color[SECRET_LEN];

    /* Flag variable for looping to attempt to read valid input. */
    int input_valid;

    /* Loop iteration variable for working with each input color */
    int i;

    /* Strict input requirements for now: only characters from the MM
     * colors-as-ASCII set are allowed, and exactly SECRET_LEN such
     * characters must be entered. */

    /* Here's a diagram of what valid input, given to us by fgets, must
     * look like, with SECRET_LEN set to 4 and where each 'C' is an
     * ASCII color:
     * chars:   C C C C \n \0
     * indices: 0 1 2 3 4  5
     *
     * This would come about by a user entering "CCCC" and then pressing
     * return. fgets keeps the newline if there's room and always adds
     * the \0 if it read anything at all.
     *
     * Checking the colors can be done simply by comparison.
     *
     * Checking the length requires analysis of the two invalid length
     * cases. Too few characters would have a newline and null
     * terminator earlier than index 4 in the array. Too many characters
     * would hide the newline from fgets and keep the null terminator at
     * index 5. So if there's no newline at index 4, we're in an invalid
     * length situation, and strlen can tell us whether we're under or
     * over.
     * */

    do {
        /* We will flag any invalid cases as we encounter them. If we
         * don't modify this flag, the input was valid and we won't
         * loop. */
        input_valid = 1;

        /* Clear the input buffer to allow for our newline comparison
         * for validating length. */
        memset(guess_ascii, 0, ASCII_BUFLEN);

        /* Get at most SECRET_LEN + 1 characters from the stream. This
         * way we can tell if an amount over SECRET_LEN characters was
         * entered using the technique described above. We also must
         * include space for the null terminator. */
        if (!fgets(guess_ascii, ASCII_BUFLEN, stdin)) {
            /* Low-level input failure; return NULL */
            fprintf(stderr, "Could not read input\n");
            return NULL;
        }

        /* Input validation: length */
        if (guess_ascii[SECRET_LEN] != '\n') {
            input_valid = 0;

            if (strlen(guess_ascii) < SECRET_LEN + 1) {
                fprintf(stderr, "Not enough characters\n");
            } else {
                fprintf(stderr, "Too many characters\n");
            }
        }

        /* Input validation: characters */
        for (i = 0; i < SECRET_LEN; i++) {
            /* Stop early if input was short */
            if (guess_ascii[i] == '\0') {
                break;
            }

            if (!ascii_is_mm_color(guess_ascii[i])) {
                input_valid = 0;

                fprintf(stderr, "'%c' is not a valid color\n",
                        guess_ascii[i]);
            }
        }

        /* Ask for new input if we're going to loop again */
        if (!input_valid) {
            fprintf(stderr, "Please try again\n");
        }
    } while (!input_valid);

    /* We now have valid input, so encode into mm_color values and
     * return. */
    for (i = 0; i < SECRET_LEN; i++) {
        guess_color[i] = ascii_to_mm_color(guess_ascii[i]);
    }

    return guess_color;
}

