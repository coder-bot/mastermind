#include "round.h"

#include "mm_color.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

struct _round {
    enum mm_color secret[SECRET_LEN];
    unsigned int turn;
    enum r_status status;
};

/* A feedback pin. */
struct pin {
    char is_black; /* 1 for black; 0 for white */
};

/* Fill r->secret with SECRET_LEN mm_color objects */
static void r_set_secret(round r);

round r_start(void)
{
    const round r = malloc(sizeof (*r));

    if (!r) {
        return NULL;
    }

    r_set_secret(r);
    r->turn = 1;
    r->status = R_ACTIVE;

    return r;
}

void r_end(const round r)
{
    free(r);
}

char *r_guess(const round r, const enum mm_color *const guess)
{
    /* To track the feedback pins as we generate them: */
    /* Up to SECRET_LEN actual pins, stored in the order we create them,
     * plus a counter */
    struct pin pins[SECRET_LEN];
    unsigned int n_pins = 0;

    /* And two sets of up to 4 pin pointers each; the sets correspond to
     * the secret and guess, and a pointer to a pin is stored in a slot
     * if and only if that pin is associated with that slot of the
     * secret or guess. */
    const struct pin *secret_pins[SECRET_LEN] = {0},
                     *guess_pins[SECRET_LEN]  = {0};

    /* For the final feedback output. */
    static char feedback[SECRET_LEN + 1];

    /* Index variables for the secret, guess, and feedback string. */
    int si, gi;
    unsigned int fi;

    /* Flag for noting whether the guess was perfect,
     * to decide whether to mark the round as won at the end */
    int guess_was_perfect;

    if (!r || !guess) {
        return NULL;
    }

    /* Implementation of Donald E. Knuth's algorithm for entering
     * feedback pins, described in his article "The Computer as Master
     * Mind" in J. Recreational Mathematics, Vol 9(1), 1976--77.
     */

    /* Let the secret be $x_1 x_2 x_3 x_4$ and
     * let the guess  be $y_1 y_2 y_3 y_4$. */

    /* 1. Enter black pins:
     * the number of positions $j$ such that $x_j = y_j$. */
    for (si = 0; si < SECRET_LEN; si++) {
        if (r->secret[si] == guess[si]) {
            /* Create and register pin */
            auto struct pin pin;
            pin.is_black = 1;
            pins[n_pins] = pin;

            /* Associate pin with the relevant positions of the
             * secret and guess */
            secret_pins[si] = guess_pins[si] = &pins[n_pins];

            /* Increment total pin count */
            n_pins++;
        }
    }

    /* 2. Enter white pins:
     * the number of positions $j$ such that $x_j \neq y_j$
     * but $x_j = y_k$ for some $k$ and $y_k$ does not have
     * any pins associated with it. */
    for (si = 0; si < SECRET_LEN; si++) {
        if (!secret_pins[si]) {
            for (gi = 0; gi < SECRET_LEN; gi++) {
                if (gi == si) {
                    continue;
                }

                if (!guess_pins[gi] && r->secret[si] == guess[gi]) {
                    /* Create and register pin; associate with
                     * positions; increment count */
                    auto struct pin pin;
                    pin.is_black = 0;
                    pins[n_pins] = pin;

                    secret_pins[si] = guess_pins[gi] = &pins[n_pins];

                    n_pins++;

                    /* Stop working on this secret position if we
                     * associated a pin with it */
                    break;
                }
            }
        }
    }

    /* Note with a perhaps more straightforward explanation:
     * 1. Enter black pins for each color match that also matches
     * position.
     * 2. Enter white pins for each color match that does *not* also
     * match position, but only if neither the secret ball nor the guess
     * ball for which the color match exists already has an associated
     * pin.
     */

    /* Make sure we collected a sensible number of pins */
    assert(n_pins <= SECRET_LEN);

    /* Create feedback output from pins */
    for (fi = 0; fi < n_pins; fi++) {
        feedback[fi] = pins[fi].is_black ? 'X' : 'x';
    }
    feedback[fi] = '\0';

    /* Update turn */
    r->turn++;


    /* Update status: check if the round was won */
    guess_was_perfect = 1;
    for (si = 0; si < SECRET_LEN; si++) {
        if (r->secret[si] != guess[si]) {
            guess_was_perfect = 0;
            break;
        }
    }

    if (guess_was_perfect) {
        r->status = R_WON;
    } else if (r->turn > MAX_TURNS) {
        r->status = R_LOST;
    }

    return feedback;
}

enum r_status r_status(c_round r)
{
    if (!r) {
        return R_NOT_A_STATUS;
    }

    return r->status;
}

unsigned int r_turn_no(const c_round r)
{
    if (!r) {
        return 0;
    }

    return r->turn;
}

const enum mm_color *r_secret(const c_round r)
{
    return r->secret;
}

static void r_set_secret(const round r)
{
    int i;

    if (!r) {
        return;
    }

    /* Seed with time, then set randomly */
    srand(time(NULL));
    for (i = 0; i < SECRET_LEN; i++) {
        r->secret[i] = rand() % MM_COLOR_TOTAL;
    }
}

