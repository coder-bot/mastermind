#include "io.h"

#include "mm_color.h"
#include "round.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Buffer length for reading an ASCII guess:
 * SECRET_LEN + room for a newline + room for a null terminator.
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

    /* If there were too many characters, we'll gobble input until we
     * find a newline so that the next requested input is clean. */
    int too_many_characters;

    /* Loop iteration variable for working with each input color */
    int i;

    /* Input requirements: only characters from the MM colors-as-ASCII
     * set are allowed, and exactly SECRET_LEN such characters must be
     * entered, followed by a newline. */

    /* Here's a diagram of what valid input, given to us by fgets with a
     * size of SECRET_LEN + 1, must look like, with SECRET_LEN set to 4
     * and where each 'C' is an ASCII color:
     * chars:   C C C C \n \0
     * indices: 0 1 2 3 4  5
     *
     * This would come about by a user entering "CCCC" and then pressing
     * return. fgets keeps the newline if there's one within the range
     * and always adds the \0 if it read anything at all.
     *
     * To check the length:
     * We'll treat EOF and error states as invalid inputs, and won't
     * analyze them further. Because of this treatment of EOF, all short
     * input that we analyze will have a newline present.
     *
     * If the user entered too few characters, the newline would appear
     * before index SECRET_LEN (4).
     *
     * If the user entered too many characters, the newline would not
     * appear at all, since fgets would reach its character limit before
     * encountering the newline.
     *
     * Therefore, the presence of a newline at index SECRET_LEN (4)
     * means the user entered the correct number of characters.
     *
     * We check the colors simply by comparison. */

    do {
        /* We will flag any invalid cases as we encounter them. If we
         * don't modify this flag, the input was valid and we won't
         * loop. */
        input_valid = 1;
        /* We'll also flag if there are too many characters. */
        too_many_characters = 0;

        /* Clear the input buffer to allow for well-defined comparisons
         * for validating length. */
        memset(guess_ascii, 0, ASCII_BUFLEN);

        /* Get at most SECRET_LEN + 1 characters from the stream. This
         * accounts for a newline character after SECRET_LEN colors. We
         * also include space for the null terminator.
         * Note that fgets returns NULL on any error and on immediate
         * EOF, but returns non-NULL on EOF after partial input
         * collection. The separate call to feof handles this last
         * situation. */
        if (!fgets(guess_ascii, ASCII_BUFLEN, stdin) || feof(stdin)) {
            /* Input error or EOF condition; return NULL */
            fprintf(stderr, "Could not read input\n");
            return NULL;
        }

        /* Input validation: length */
        if (guess_ascii[SECRET_LEN] != '\n') {
            input_valid = 0;

            for (i = 0; i < SECRET_LEN; i++) {
                if (guess_ascii[i] == '\n') {
                    fprintf(stderr, "Not enough characters\n");
                    break;
                }
            }
            if (i == SECRET_LEN) {
                fprintf(stderr, "Too many characters\n");
                too_many_characters = 1;
            }
        } else {
            /* Input validation: characters. We only do this if we
             * passed length validation. */
            for (i = 0; i < SECRET_LEN; i++) {
                if (!ascii_is_mm_color(guess_ascii[i])) {
                    input_valid = 0;

                    fprintf(stderr, "'%c' is not a valid color\n",
                            guess_ascii[i]);
                }
            }
        }

        /* Ask for new input if we're going to loop again */
        if (!input_valid) {
            /* But first, if there were too many characters this time,
             * clear the input buffer through the newline */
            if (too_many_characters) {
                int c;
                while ((c = getchar()) != '\n') {
                    if (c == EOF) {
                        fprintf(stderr, "Could not read further "
                                "input\n");
                        return NULL;
                    }
                }
            }

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

