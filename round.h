#include "mm_color.h"

/* A round of the game. */
typedef struct _round *round;
typedef const struct _round *c_round;

/* A sequence of 4 colors makes a secret. */
#define SECRET_LEN (4)

/* 10 guesses max */
#define MAX_TURNS (10u)

enum r_status {
    R_NOT_A_STATUS = -1,
    R_ACTIVE = 0,
    R_WON,
    R_LOST
};

/* Start and return a new round. */
round r_start(void);

/* End a round. The object r is invalid after this call. */
void r_end(round r);

/* Submit a guess of SECRET_LEN colors.
 * Returns SECRET_LEN characters of feedback; returns NULL if either
 * parameter is NULL. */
char *r_guess(round r, const enum mm_color *guess);

/* Get round status. Returns R_NOT_A_STATUS if and only if r is NULL. */
enum r_status r_status(c_round r);

/* Get current turn number n, n > 0. Returns 0 if r is NULL. */
unsigned int r_turn_no(c_round r);

/* For at the end: return the secret. */
const enum mm_color *r_secret(c_round r);

