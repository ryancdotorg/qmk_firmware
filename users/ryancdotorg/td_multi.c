/* Copyright (c) 2020 Ryan Castellucci @ryancdotorg GPLv2+ */
#include QMK_KEYBOARD_H
#include "td_multi.h"

#include <print.h>

/* This just helps keep things clean. */
#ifdef CONSOLE_ENABLE
#  define DPRINT(S) dprint(S)
#  define DPRINTF(...) dprintf(__VA_ARGS__)
#else
#  define DPRINT(S) do {} while(0)
#  define DPRINTF(...) do {} while(0)
#endif

/* DO NOT CHANGE - These values are relied upon by the bitwise math.
 * The two LSB of the multi state hold one of these values, the rest hold the
 * number of presses. For example, after a "tap, tap, hold", the value will be
 * (3 << 2) + 2, or 0b000011_10 = 14 */
#define MODE_TAP   0
#define MODE_MULTI 1
#define MODE_HOLD  2

/* Inspired by the "Quad Function Tap-Dance" by @danielggordan.
 * https://github.com/qmk/qmk_firmware/blob/master/users/gordon/gordon.c
 * This variation can handle multiple sequences with just one set of functions
 * using user_data, and handles any number of taps (within reason). */
static int td_multi_evaluate(qk_tap_dance_state_t *state, uint8_t max_multi) {
    typeof(state->count) count = state->count;
    DPRINTF("td_multi_evaluate: count=%d ", count);
    int ret = 127; /* default return value */
    if (count > 0 && count <= 15) {
        ret = count << 2;
        if (count > 1 && count <= max_multi && state->interrupted) {
            /* Something else was pressed after multiple taps - return MULTI
             * action. This is to distinguish between typing "pepper", and
             * actually wanting a double tap action when hitting "pp". Suggested
             * use case for this return value is when you want to send multiple
             * keystrokes of the key, rather than the 'N taps' action/macro. */
            ret += MODE_MULTI;
        } else if (!state->interrupted && state->pressed) {
            /* Key has not been interrupted, and is still held - return HOLD
             * action. */
            ret += MODE_HOLD;
        } else {
            /* Default - send one tap of the bound key */
            ret += MODE_TAP;
        }
    }
    DPRINTF("ret=%d\n", ret);
    return ret;
}

void td_multi_finished(qk_tap_dance_state_t *state, void *user_data) {
    DPRINT("td_multi_finished\n");
    td_multi_t *multi = (td_multi_t *)user_data;
    int ret = multi->ret = td_multi_evaluate(state, 2);
    int mode = ret & 3;
    DPRINTF("td_multi_finished ret=%d mode=%d\n", ret, mode);
    if (ret > 3) {
        ret -= 4;
        if ((ret >> 1) < multi->n_keys) {
            if (mode == MODE_TAP || mode == MODE_HOLD) {
                register_code(multi->keys[ret >> 1]);
            } else if (mode == MODE_MULTI) {
                /* just press the key however many times */
                while (ret > 0) {
                    register_code(multi->keys[0]);
                    unregister_code(multi->keys[0]);
                    ret -= 4;
                }
            }
        } else if (ret == multi->n_keys << 1) {
            /* e.g. if we had (KC_1, KC_2, KC_3) and got "tap, hold", hold KC_3 */
            register_code(multi->keys[(ret >> 1) & (~1)]);
            multi->ret -= MODE_HOLD;
        }
    }
}

void td_multi_reset(qk_tap_dance_state_t *state, void *user_data) {
    DPRINT("td_multi_reset ");
    td_multi_t *multi = (td_multi_t *)user_data;
    int ret = multi->ret;
    int mode = ret & 3;
    DPRINTF("ret=%d mode=%d\n", ret, mode);
    if (ret > 3) {
        ret -= 4;
        if ((ret >> 1) < multi->n_keys) {
            if (mode == MODE_TAP || mode == MODE_HOLD) {
                unregister_code(multi->keys[ret >> 1]);
            }
        }
    }
    multi->ret = 0;
}
