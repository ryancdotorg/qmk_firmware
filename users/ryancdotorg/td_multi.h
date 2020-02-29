/* Copyright (c) 2020 Ryan Castellucci @ryancdotorg GPLv2+ */
#pragma once

typedef struct {
    int ret;
    int n_keys;
    uint16_t *keys;
} td_multi_t;

/* For the __VA_ARGS__ stuff, please read the cpp docs here:
 * https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html
 *
 * The magic bit here is combining the c preprocessor and compiler to compute
 * the number of arguments at build time. Best to explain this with an example.
 *
 * Say we use ACTION_TAP_DANCE_MULTI(KC_A, KC_B, KC_C, KC_D) - we'll get this:
 *
 * .n_keys = sizeof(((uint16_t[]){KC_A, KC_B, KC_C, KC_D}))/sizeof(uint16_t)
 *
 * The compiler calculates sizeof at build time. Since a single uint16_t value
 * is 2 bytes (on any sane system...), and we have a four element array literal,
 * we should get "8/2" here. The compiler will the optimize that to just "4". */

#define ACTION_TAP_DANCE_MULTI(...) \
{ \
    .fn = {NULL, td_multi_finished, td_multi_reset, }, \
    .user_data = (void *)&((td_multi_t){ \
        .n_keys = sizeof(((uint16_t[]){__VA_ARGS__}))/sizeof(uint16_t), \
        .keys = ((uint16_t[]){__VA_ARGS__}), .ret = 0, \
    }), \
}

void td_multi_finished(qk_tap_dance_state_t *state, void *user_data);
void td_multi_reset(qk_tap_dance_state_t *state, void *user_data);
