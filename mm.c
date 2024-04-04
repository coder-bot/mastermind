#include "io.h"
#include "round.h"
#include "mm_color.h"

#include <stdio.h>
#include <string.h>

int main()
{
    /* The round object for this game. */
    round r;

    /* To hold a pointer to the secret for this round. */
    const enum mm_color *secret;

    /* To hold the current turn number. */
    unsigned int turn_no;

    /* Say hello ... */
    banner();
    print_rules();

    /* Initialize round */
    r = r_start();

    /* Stash the secret for printing here (debug) or at the end
     * (release) */
    secret = r_secret(r);

#if 0
    /* Debugging: print the secret */
    print_colors_as_emoji(secret);
    printf("\n");
#endif

    /* Run loop */
    while (r_status(r) == R_ACTIVE) {
        /* Guess and feedback for this turn */
        enum mm_color *guess;
        char *feedback;

        /* i for iterating over the feedback,
         * j for iterating to print spaces before the feedback */
        int i, j;

        /* Update turn number and use it to prompt for a guess */
        turn_no = r_turn_no(r);
        printf("%u: ", turn_no);

        if (!(guess = get_guess())) {
            /* Low-level input failure */
            return 1;
        }

        /* Echo back guess as emoji */
        print_colors_as_emoji(guess);
        printf("\n");

        /* Submit guess */
        feedback = r_guess(r, guess);

        /* Print feedback */
        for (i = 0; i < (int) strlen(feedback); i++) {
            if (i % (SECRET_LEN / 2) == 0) {
                if (i != 0) {
                    printf("\n");
                }

                /* Add space before each line of feedback so it appears
                 * to the right of the emoji */
                for (j = 0; j < 2 * SECRET_LEN + 1; j++) {
                    printf(" ");
                }
            }

            printf("%c", feedback[i]);
        }
        printf("\n");
    }

    /* Done! Reveal the secret. */
    printf("\n");
    print_colors_as_emoji(secret);
    printf("\n\n");

    if (r_status(r) == R_WON) {
        printf("You won in %u turns!\n", turn_no);
    } else {
        printf("You lost!\n");
    }

    r_end(r);

    return 0;
}

